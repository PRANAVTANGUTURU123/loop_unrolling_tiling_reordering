# Loop Unrolling, Tiling, and Reordering Project

This project explores **loop optimization techniques** such as **loop unrolling, tiling, and loop reordering**.  
It generates C programs with different optimization combinations, compiles them, runs them, and records their execution times for analysis.

---

## 📂 Folder Structure

- **clooprunner.py** → Main script to generate all tiled loop C programs inside `tiled_c_outputs/`.
- **looprun.py** → Compiles and runs the generated C files, records execution times, and writes results to `loop_runtime_results.csv`.
- **find_order.py** → Generates all possible loop order permutations.
- **tiling.py** → Applies tiling transformation to loops with given tile sizes.
- **loop_unroll.py** → Applies loop unrolling to loops with a given unroll factor.
- **reorder.py** → Reorders nested loops into different permutations.
- **utils.py** (if present) → Helper functions used across scripts.
- **tiled_c_outputs/** → Auto-generated folder that stores C code outputs from `clooprunner.py`.
- **loop_runtime_results.csv** → CSV file that stores runtime results after executing generated C programs.

---

## ⚙️ How It Works

1. **clooprunner.py**
   - Generates all possible loop-tiling/unrolling/reordering variations.
   - Saves the generated C programs in `tiled_c_outputs/`.

2. **looprun.py**
   - Compiles each C program inside `tiled_c_outputs/` using `gcc` or `clang`.
   - Executes the binaries.
   - Records execution times into `loop_runtime_results.csv`.

3. **find_order.py**
   - Generates permutations of loop ordering.

4. **tiling.py**
   - Transforms loops into tiled versions with user-defined tile sizes.

5. **loop_unroll.py**
   - Applies loop unrolling with a given unroll factor.

6. **reorder.py**
   - Applies different loop reordering strategies.

---

## 🚀 How to Run

### 1. Generate tiled C outputs
```bash
python3 clooprunner.py
