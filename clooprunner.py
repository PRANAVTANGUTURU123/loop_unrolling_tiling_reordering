# clooprunner.py (cycle counting version)
import os
import re
from typing import List, Dict, Tuple, Optional
from find_order import generate_all_combinations

PROJECT_ROOT = os.path.dirname(os.path.abspath(__file__))
TEST_C = os.path.join(PROJECT_ROOT, "test.c")
OUTPUT_DIR = os.path.join(PROJECT_ROOT, "tiled_c_outputs")

TILE_VAR = {"i": "i_t", "j": "j_t", "k": "k_t"}
DIM_NAMES = {"i": "DEPTH", "j": "HEIGHT", "k": "WIDTH"}

# -----------------------
# Helpers for parsing
# -----------------------
def strip_comments(src: str) -> str:
    src = re.sub(r"/\*.*?\*/", "", src, flags=re.DOTALL)
    src = re.sub(r"//.*?$", "", src, flags=re.MULTILINE)
    return src

def match_brace(src: str, open_idx: int) -> int:
    depth = 0
    for i in range(open_idx, len(src)):
        if src[i] == '{':
            depth += 1
        elif src[i] == '}':
            depth -= 1
            if depth == 0:
                return i
    return -1

def find_for_open_brace(src: str, start: int, var: str) -> Optional[int]:
    m = re.search(rf"for\s*\(\s*int\s+{var}\b", src[start:])
    if not m:
        return None
    pos = start + m.start()
    open_idx = src.find("{", pos)
    return open_idx if open_idx != -1 else None

def extract_inner_k_body(content: str) -> str:
    start_pos = content.find("Performing operations")
    scan_src = content[start_pos:] if start_pos != -1 else content
    tmp = strip_comments(scan_src)

    i_open = find_for_open_brace(tmp, 0, "i")
    if i_open is None:
        return ""
    i_close = match_brace(tmp, i_open)
    j_open = find_for_open_brace(tmp, i_open + 1, "j")
    if j_open is None:
        return ""
    j_close = match_brace(tmp, j_open)
    k_open = find_for_open_brace(tmp, j_open + 1, "k")
    if k_open is None:
        return ""
    k_close = match_brace(tmp, k_open)

    body = tmp[k_open + 1:k_close]
    body = re.sub(r'printf\s*\(.*?\);\s*', '', body, flags=re.DOTALL)
    return body.strip()

def extract_arrays_and_loops(file_path: str):
    with open(file_path, "r") as f:
        content = f.read()
    array_pattern = re.compile(r"(?:static\s+)?int\s+(\w+)\s*\[\s*(\w+)\s*\]\s*\[\s*(\w+)\s*\]\s*\[\s*(\w+)\s*\]\s*;")
    arrays = array_pattern.findall(content)

    dims: Dict[str,int] = {}
    for m in re.finditer(r"#define\s+(\w+)\s+(\d+)", content):
        dims[m.group(1)] = int(m.group(2))

    loop_body = extract_inner_k_body(content)
    return arrays, dims, loop_body

def emit(buf, indent, line):
    buf.append(" " * indent + line + "\n")

def token_safe_replace(line: str, var: str, repl: str) -> str:
    return re.sub(rf"\b{re.escape(var)}\b", repl, line)

def open_block(buf, indent, header: str):
    emit(buf, indent, (header + " {") if header else "{")
    return indent + 4

def close_block(buf, indent):
    indent -= 4
    emit(buf, indent, "}")
    return indent

# -----------------------
# Code generation
# -----------------------
def build_program(order: Tuple[str, ...], tile_size: int, unroll: int, arrays, dims, loop_body: str) -> str:
    buf = []
    # headers with cycle counting function
    buf.append(
        "#include <stdio.h>\n"
        "#include <stdint.h>\n\n"
        "// Cycle counting function\n"
        "static inline unsigned long long rdtsc_serialized(void) {\n"
        "    unsigned int low, high;\n"
        "    asm volatile (\"cpuid\" : : : \"%rax\", \"%rbx\", \"%rcx\", \"%rdx\");\n"
        "    asm volatile (\"rdtsc\" : \"=a\" (low), \"=d\" (high));\n"
        "    return ((unsigned long long)high << 32) | low;\n"
        "}\n\n"
    )
    
    # macros
    buf.append(f"#define TILE_SIZE {tile_size}\n#define UNROLL {unroll}\n")
    for dim_name, size in dims.items():
        buf.append(f"#define {dim_name} {size}\n")
    for name, d0, d1, d2 in arrays:
        buf.append(f"static int {name}[{d0}][{d1}][{d2}];\n")

    # main()
    buf.append("\nint main() {\n")
    indent = 4

    emit(buf, indent, "unsigned long long start_cycles, end_cycles;")
    
    # Init (same as before)
    emit(buf, indent, "for (int i=0;i<DEPTH;i++)")
    emit(buf, indent + 4, "for (int j=0;j<HEIGHT;j++)")
    emit(buf, indent + 8, "for (int k=0;k<WIDTH;k++)")
    emit(buf, indent + 12, "data[i][j][k]=i+j+k;")
    buf.append("\n")
    
    emit(buf, indent, "start_cycles = rdtsc_serialized();")

    # determine innermost point loop
    inner_var = None
    for v in reversed(order):
        if not v.endswith("_t"):
            inner_var = v
            break

    # loops in chosen order
    for v in order:
        if v.endswith("_t"):
            base = v[0]
            indent = open_block(buf, indent, f"for (int {v} = 0; {v} < {DIM_NAMES[base]}; {v} += TILE_SIZE)")
        else:
            base = v
            tvar = {"i": "i_t", "j": "j_t", "k": "k_t"}[base]
            emit(buf, indent, f"int {base}_end = {tvar} + TILE_SIZE; if ({base}_end > {DIM_NAMES[base]}) {base}_end = {DIM_NAMES[base]};")

            if base == inner_var and unroll > 1:
                emit(buf, indent, f"int {base} = {tvar};")
                indent = open_block(buf, indent, f"for (; {base} + (UNROLL - 1) < {base}_end; {base} += {unroll})")
                for u in range(unroll):
                    indent = open_block(buf, indent, "")
                    for line in loop_body.splitlines():
                        if line.strip():
                            emit(buf, indent, token_safe_replace(line, base, f"{base}+{u}"))
                    indent = close_block(buf, indent)
                indent = close_block(buf, indent)  # close unrolled for

                indent = open_block(buf, indent, f"if ({base} < {base}_end)")
                indent = open_block(buf, indent, f"for (; {base} < {base}_end; ++{base})")
                for line in loop_body.splitlines():
                    if line.strip():
                        emit(buf, indent, line)
                indent = close_block(buf, indent)  # for
                indent = close_block(buf, indent)  # if
            else:
                indent = open_block(buf, indent, f"for (int {base} = {tvar}; {base} < {base}_end; ++{base})")

    # close all opened blocks until main body
    while indent > 4:
        indent = close_block(buf, indent)

    emit(buf, indent, "end_cycles = rdtsc_serialized();")
    emit(buf, indent, 'printf("⏱️ Loop cycles: %llu\\n", end_cycles - start_cycles);')
    emit(buf, indent, "return 0;")
    emit(buf, 0, "}")

    return "".join(buf)

def write_all_files():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    arrays, dims, loop_body = extract_arrays_and_loops(TEST_C)
    combos = generate_all_combinations()

    print(f"Generating {len(combos)} C files...")
    print("Organization: Each ordering -> Each tile size -> All unroll factors")
    print("=" * 60)

    current_order = None
    current_tile = None

    for combo in combos:
        if combo['order_idx'] != current_order:
            current_order = combo['order_idx']
            current_tile = None
            print(f"\nOrdering {combo['order_idx']}: {combo['order']}")

        if combo['tile_size'] != current_tile:
            current_tile = combo['tile_size']
            print(f"  Tile size {combo['tile_size']:3d}: ", end="")

        print(f"{combo['unroll']:2d}", end=" ")

        code = build_program(
            tuple(combo["order"]),
            combo["tile_size"],
            combo["unroll"],
            arrays,
            dims,
            loop_body
        )
        fname = f"{combo['combo_idx']}_ordering_{combo['order_idx']}_tile_{combo['tile_idx']}_unroll_{combo['unroll']}.c"
        with open(os.path.join(OUTPUT_DIR, fname), "w") as f:
            f.write(code)

    print(f"\n\nGenerated all {len(combos)} files in {OUTPUT_DIR}/")

if __name__ == "__main__":
    write_all_files()