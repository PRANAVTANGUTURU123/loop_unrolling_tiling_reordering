# 🚀 Loop Unrolling, Tiling & Reordering Analyser 🚀  

Welcome! This project automates the process of experimenting with **loop unrolling, tiling, and reordering** on C code.  
It generates multiple optimized versions of your program, compiles them, benchmarks runtime performance, and helps you identify the **fastest configuration 🏆**.  

---

## ⚙️ Step 1: Generate Tiled Code Variants  

The first step is to generate different tiled/unrolled/reordered versions of your base C code.  
This is handled by **`looprun.py`** using the tiling/ordering strategies from **`find_order.py`**.  

### 🔹 What it does:
- Reads your base C file (e.g., `test.c`).  
- Produces **many tiled/unrolled variants** by changing tile sizes and loop orders.  
- Saves all generated files in a folder called **`tiled_c_outputs/`**.  

### ▶️ Run this command:
```bash
python3 looprun.py
