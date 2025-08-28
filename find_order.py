# find_order.py
from typing import List, Dict, Tuple
import itertools

def generate_legal_permutations(pairs: List[Tuple[str, str]]):
    """
    Generate all legal sequences respecting the pair order (tile -> point).
    E.g., [('i_t','i'),('j_t','j'),('k_t','k')] generates 90 permutations.
    """
    n = len(pairs)
    placed = [0] * n
    seq: List[str] = []
    
    def backtrack():
        if len(seq) == 2 * n:
            yield tuple(seq)
            return
        
        for p, (a, b) in enumerate(pairs):
            if placed[p] == 0:
                seq.append(a)
                placed[p] = 1
                yield from backtrack()
                placed[p] = 0
                seq.pop()
            elif placed[p] == 1:
                seq.append(b)
                placed[p] = 2
                yield from backtrack()
                placed[p] = 1
                seq.pop()
    
    yield from backtrack()

def generate_all_combinations() -> List[Dict]:
    """
    Generate combinations organized as:
    - For each ordering
      - For each tile size
        - All unroll factors (2, 4, 6, ..., 20)
    """
    loop_dimension_pairs = [('i_t', 'i'), ('j_t', 'j'), ('k_t', 'k')]
    TILE_SIZES = [8, 16, 32, 64, 128, 256]
    UNROLL_FACTORS = list(range(2, 21, 2))  # 2,4,6,...,20
    
    orders = list(generate_legal_permutations(loop_dimension_pairs))
    combos = []
    combo_idx = 0
    
    for order_idx, order in enumerate(orders, 1):
        for tile_idx, tile_size in enumerate(TILE_SIZES, 1):
            for unroll in UNROLL_FACTORS:
                combo_idx += 1
                combos.append({
                    "combo_idx": combo_idx,
                    "order_idx": order_idx,
                    "tile_idx": tile_idx,
                    "order": order,
                    "tile_size": tile_size,
                    "unroll": unroll
                })
    
    return combos

def print_combination_summary():
    """Print a summary showing the organization structure"""
    loop_dimension_pairs = [('i_t', 'i'), ('j_t', 'j'), ('k_t', 'k')]
    TILE_SIZES = [8, 16, 32, 64, 128, 256]
    UNROLL_FACTORS = list(range(2, 21, 2))
    
    orders = list(generate_legal_permutations(loop_dimension_pairs))
    
    print(f"Total orderings: {len(orders)}")
    print(f"Tile sizes: {TILE_SIZES}")
    print(f"Unroll factors: {UNROLL_FACTORS}")
    print(f"Total combinations: {len(orders) * len(TILE_SIZES) * len(UNROLL_FACTORS)}")
    print("\nFirst few orderings:")
    for i, order in enumerate(orders[:5], 1):
        print(f"  Ordering {i}: {order}")

if __name__ == "__main__":
    print_combination_summary()
