# looprun.py (cycle counting version - batch CSV writing every 20 runs)
import os
import subprocess
import csv
from find_order import generate_all_combinations

OUTPUT_FOLDER = "tiled_c_outputs"
CSV_FILE = "loop_cycle_results.csv"
BATCH_SIZE = 20  # Write to CSV every 20 runs

def try_compile(c_file: str, exe_file: str) -> bool:
    toolchains = [
        ["clang", "-O3", "-march=native", c_file, "-o", exe_file],
        ["gcc",   "-O3", "-march=native", c_file, "-o", exe_file],
    ]
    last_err = None
    for cmd in toolchains:
        try:
            subprocess.run(cmd, check=True, capture_output=True, text=True)
            return True
        except FileNotFoundError:
            continue
        except subprocess.CalledProcessError as e:
            last_err = e
    if last_err:
        print(f"❌ Compilation failed for {c_file}.\n"
              f"stderr:\n{last_err.stderr.strip() if last_err.stderr else '(no stderr)'}\n")
    else:
        print(f"❌ Compilation failed for {c_file}: no supported C compiler found (clang/gcc).")
    return False

def run_executable_and_get_cycles(exe_path: str, timeout_s: int = 120):
    try:
        result = subprocess.run([exe_path], capture_output=True, text=True, check=True, timeout=timeout_s)
        for line in result.stdout.splitlines():
            if "Loop cycles" in line:  # Changed from "Loop runtime" to "Loop cycles"
                parts = line.split(":")
                if len(parts) > 1:
                    # Extract the number (cycles are integers)
                    cycle_str = parts[1].strip()
                    return int(cycle_str)
        print(f"⚠️ Warning: Could not find cycle count in output for {exe_path}")
        return None
    except subprocess.TimeoutExpired:
        print(f"⏱️ Timeout running {exe_path} (>{timeout_s}s); skipping.")
        return None
    except subprocess.CalledProcessError as e:
        print(f"💥 Error running {exe_path}: {e}\nstderr:\n{e.stderr}")
        return None
    except (ValueError, Exception) as e:
        print(f"⚠️ Error parsing cycles from {exe_path}: {e}")
        return None

def count_loop_dependencies(loop_body: str) -> int:
    """Count the number of data dependencies in the loop body."""
    import re
    # Find all array references like data[...][...][...]
    array_refs = re.findall(r'data\[([^\]]+)\]\[([^\]]+)\]\[([^\]]+)\]', loop_body)
    # Count unique dependency patterns
    unique_deps = set()
    for ref in array_refs:
        pattern = tuple(ref)
        unique_deps.add(pattern)
    return len(unique_deps)

def write_batch_to_csv(csv_path: str, batch_data: list, write_header: bool = False):
    """Write a batch of results to CSV file"""
    with open(csv_path, "a", newline="") as csv_file:
        fieldnames = [
            "Index", "Order_Index", "Tile_Index", "Tile_Size",
            "Loop_Order", "Unroll_Factor", "Execution_Cycles", "Loop_Dependencies_Count"
        ]
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        
        if write_header:
            writer.writeheader()
        
        for row in batch_data:
            writer.writerow(row)

def compile_and_run_all():
    if not os.path.exists(OUTPUT_FOLDER):
        print(f"❌ Error: {OUTPUT_FOLDER} not found. Run clooprunner.py first.")
        return

    combinations = generate_all_combinations()
    total_files = len(combinations)

    csv_path = os.path.join(OUTPUT_FOLDER, CSV_FILE)
    write_header = not os.path.exists(csv_path)
    
    # Get loop body for dependency analysis
    test_c_path = "test.c"
    loop_dependencies_count = 0
    
    if os.path.exists(test_c_path):
        with open(test_c_path, "r") as f:
            content = f.read()
            start_pos = content.find("Performing operations")
            if start_pos != -1:
                loop_section = content[start_pos:]
                loop_dependencies_count = count_loop_dependencies(loop_section)

    print(f"🚀 Compiling and running {total_files} programs...")
    print(f"📊 Loop dependencies detected: {loop_dependencies_count}")
    print(f"💾 Writing results to CSV every {BATCH_SIZE} runs")
    print(f"⚙️ Measuring execution cycles (not time)")
    print("=" * 60)

    batch_data = []
    successful_runs = 0

    for i, combo in enumerate(combinations):
        c_file = os.path.join(
            OUTPUT_FOLDER,
            f"{combo['combo_idx']}_ordering_{combo['order_idx']}_tile_{combo['tile_idx']}_unroll_{combo['unroll']}.c"
        )
        exe_file = c_file.replace(".c", "")

        if not os.path.exists(c_file):
            print(f"⚠️ Warning: {c_file} not found, skipping...")
            continue

        if not try_compile(c_file, exe_file):
            continue

        cycles = run_executable_and_get_cycles(exe_file)
        if cycles is not None:
            successful_runs += 1
            
            # IMMEDIATE terminal output - print every single run result
            print(f"[{i+1:4d}/{total_files}] {os.path.basename(c_file)} → {cycles:,} cycles "
                  f"(Order {combo['order_idx']}, Tile {combo['tile_size']}, Unroll {combo['unroll']})")

            # Collect data for batch CSV writing
            row = {
                "Index": combo["combo_idx"],
                "Order_Index": combo["order_idx"],
                "Tile_Index": combo["tile_idx"],
                "Tile_Size": combo["tile_size"],
                "Loop_Order": "".join(combo["order"]),
                "Unroll_Factor": combo["unroll"],
                "Execution_Cycles": cycles,
                "Loop_Dependencies_Count": loop_dependencies_count
            }
            
            batch_data.append(row)

            # Write to CSV only every BATCH_SIZE successful runs
            if len(batch_data) >= BATCH_SIZE:
                write_batch_to_csv(csv_path, batch_data, write_header)
                write_header = False  # Only write header once
                print(f"📝 Batch CSV update: Wrote {len(batch_data)} results to file (Total saved: {successful_runs})")
                batch_data.clear()

        try:
            if os.path.exists(exe_file):
                os.remove(exe_file)
        except OSError:
            pass

    # Write any remaining data in the final batch
    if batch_data:
        write_batch_to_csv(csv_path, batch_data, write_header)
        print(f"📝 Final CSV update: Wrote {len(batch_data)} results to file")

    print(f"\n✅ Complete! Total successful runs: {successful_runs}")
    print(f"📈 Cycle results saved to {csv_path}")

if __name__ == "__main__":
    compile_and_run_all()