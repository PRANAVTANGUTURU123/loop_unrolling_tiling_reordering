#include <stdio.h>
#include <stdint.h>

// Cycle counting function
static inline unsigned long long rdtsc_serialized(void) {
    unsigned int low, high;
    asm volatile ("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx");
    asm volatile ("rdtsc" : "=a" (low), "=d" (high));
    return ((unsigned long long)high << 32) | low;
}

#define TILE_SIZE 16
#define UNROLL 8
#define DEPTH 1024
#define HEIGHT 1024
#define WIDTH 1024
static int data[DEPTH][HEIGHT][WIDTH];

int main() {
    unsigned long long start_cycles, end_cycles;
    for (int i=0;i<DEPTH;i++)
        for (int j=0;j<HEIGHT;j++)
            for (int k=0;k<WIDTH;k++)
                data[i][j][k]=i+j+k;

    start_cycles = rdtsc_serialized();
    for (int k_t = 0; k_t < WIDTH; k_t += TILE_SIZE) {
        for (int i_t = 0; i_t < DEPTH; i_t += TILE_SIZE) {
            for (int j_t = 0; j_t < HEIGHT; j_t += TILE_SIZE) {
                int i_end = i_t + TILE_SIZE; if (i_end > DEPTH) i_end = DEPTH;
                for (int i = i_t; i < i_end; ++i) {
                    int k_end = k_t + TILE_SIZE; if (k_end > WIDTH) k_end = WIDTH;
                    for (int k = k_t; k < k_end; ++k) {
                        int j_end = j_t + TILE_SIZE; if (j_end > HEIGHT) j_end = HEIGHT;
                        int j = j_t;
                        for (; j + (UNROLL - 1) < j_end; j += 8) {
                            {
                                if (i > 1 && j+0 > 1 && k > 1 && i < DEPTH - 2 && j+0 < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j+0][k];                   
                                        int candidate2 = data[i][k][j+0] + data[k][j+0][i];   
                                        int candidate3 = data[i-1][j+0][k] + data[i][j+0-1][k]; 
                                        int candidate4 = data[i][j+0][k-1] + data[i-1][j+0-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j+0][k] = new_val;
                                        if (i > 0 && j+0 > 0)
                                          data[i][j+0][k] = (data[i][j+0][k] < data[i-1][j+0][k] + data[i][j+0-1][k])
                                                           ? data[i][j+0][k] : data[i-1][j+0][k] + data[i][j+0-1][k];
                                        if (j+0 > 0 && k > 0)
                                          data[i][j+0][k] = (data[i][j+0][k] < data[i][j+0-1][k] + data[i][j+0][k-1])
                                                           ? data[i][j+0][k] : data[i][j+0-1][k] + data[i][j+0][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j+0][k] = (data[i][j+0][k] < data[i-1][j+0][k] + data[i][j+0][k-1])
                                                           ? data[i][j+0][k] : data[i-1][j+0][k] + data[i][j+0][k-1];
                                      }
                            }
                            {
                                if (i > 1 && j+1 > 1 && k > 1 && i < DEPTH - 2 && j+1 < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j+1][k];                   
                                        int candidate2 = data[i][k][j+1] + data[k][j+1][i];   
                                        int candidate3 = data[i-1][j+1][k] + data[i][j+1-1][k]; 
                                        int candidate4 = data[i][j+1][k-1] + data[i-1][j+1-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j+1][k] = new_val;
                                        if (i > 0 && j+1 > 0)
                                          data[i][j+1][k] = (data[i][j+1][k] < data[i-1][j+1][k] + data[i][j+1-1][k])
                                                           ? data[i][j+1][k] : data[i-1][j+1][k] + data[i][j+1-1][k];
                                        if (j+1 > 0 && k > 0)
                                          data[i][j+1][k] = (data[i][j+1][k] < data[i][j+1-1][k] + data[i][j+1][k-1])
                                                           ? data[i][j+1][k] : data[i][j+1-1][k] + data[i][j+1][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j+1][k] = (data[i][j+1][k] < data[i-1][j+1][k] + data[i][j+1][k-1])
                                                           ? data[i][j+1][k] : data[i-1][j+1][k] + data[i][j+1][k-1];
                                      }
                            }
                            {
                                if (i > 1 && j+2 > 1 && k > 1 && i < DEPTH - 2 && j+2 < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j+2][k];                   
                                        int candidate2 = data[i][k][j+2] + data[k][j+2][i];   
                                        int candidate3 = data[i-1][j+2][k] + data[i][j+2-1][k]; 
                                        int candidate4 = data[i][j+2][k-1] + data[i-1][j+2-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j+2][k] = new_val;
                                        if (i > 0 && j+2 > 0)
                                          data[i][j+2][k] = (data[i][j+2][k] < data[i-1][j+2][k] + data[i][j+2-1][k])
                                                           ? data[i][j+2][k] : data[i-1][j+2][k] + data[i][j+2-1][k];
                                        if (j+2 > 0 && k > 0)
                                          data[i][j+2][k] = (data[i][j+2][k] < data[i][j+2-1][k] + data[i][j+2][k-1])
                                                           ? data[i][j+2][k] : data[i][j+2-1][k] + data[i][j+2][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j+2][k] = (data[i][j+2][k] < data[i-1][j+2][k] + data[i][j+2][k-1])
                                                           ? data[i][j+2][k] : data[i-1][j+2][k] + data[i][j+2][k-1];
                                      }
                            }
                            {
                                if (i > 1 && j+3 > 1 && k > 1 && i < DEPTH - 2 && j+3 < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j+3][k];                   
                                        int candidate2 = data[i][k][j+3] + data[k][j+3][i];   
                                        int candidate3 = data[i-1][j+3][k] + data[i][j+3-1][k]; 
                                        int candidate4 = data[i][j+3][k-1] + data[i-1][j+3-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j+3][k] = new_val;
                                        if (i > 0 && j+3 > 0)
                                          data[i][j+3][k] = (data[i][j+3][k] < data[i-1][j+3][k] + data[i][j+3-1][k])
                                                           ? data[i][j+3][k] : data[i-1][j+3][k] + data[i][j+3-1][k];
                                        if (j+3 > 0 && k > 0)
                                          data[i][j+3][k] = (data[i][j+3][k] < data[i][j+3-1][k] + data[i][j+3][k-1])
                                                           ? data[i][j+3][k] : data[i][j+3-1][k] + data[i][j+3][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j+3][k] = (data[i][j+3][k] < data[i-1][j+3][k] + data[i][j+3][k-1])
                                                           ? data[i][j+3][k] : data[i-1][j+3][k] + data[i][j+3][k-1];
                                      }
                            }
                            {
                                if (i > 1 && j+4 > 1 && k > 1 && i < DEPTH - 2 && j+4 < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j+4][k];                   
                                        int candidate2 = data[i][k][j+4] + data[k][j+4][i];   
                                        int candidate3 = data[i-1][j+4][k] + data[i][j+4-1][k]; 
                                        int candidate4 = data[i][j+4][k-1] + data[i-1][j+4-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j+4][k] = new_val;
                                        if (i > 0 && j+4 > 0)
                                          data[i][j+4][k] = (data[i][j+4][k] < data[i-1][j+4][k] + data[i][j+4-1][k])
                                                           ? data[i][j+4][k] : data[i-1][j+4][k] + data[i][j+4-1][k];
                                        if (j+4 > 0 && k > 0)
                                          data[i][j+4][k] = (data[i][j+4][k] < data[i][j+4-1][k] + data[i][j+4][k-1])
                                                           ? data[i][j+4][k] : data[i][j+4-1][k] + data[i][j+4][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j+4][k] = (data[i][j+4][k] < data[i-1][j+4][k] + data[i][j+4][k-1])
                                                           ? data[i][j+4][k] : data[i-1][j+4][k] + data[i][j+4][k-1];
                                      }
                            }
                            {
                                if (i > 1 && j+5 > 1 && k > 1 && i < DEPTH - 2 && j+5 < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j+5][k];                   
                                        int candidate2 = data[i][k][j+5] + data[k][j+5][i];   
                                        int candidate3 = data[i-1][j+5][k] + data[i][j+5-1][k]; 
                                        int candidate4 = data[i][j+5][k-1] + data[i-1][j+5-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j+5][k] = new_val;
                                        if (i > 0 && j+5 > 0)
                                          data[i][j+5][k] = (data[i][j+5][k] < data[i-1][j+5][k] + data[i][j+5-1][k])
                                                           ? data[i][j+5][k] : data[i-1][j+5][k] + data[i][j+5-1][k];
                                        if (j+5 > 0 && k > 0)
                                          data[i][j+5][k] = (data[i][j+5][k] < data[i][j+5-1][k] + data[i][j+5][k-1])
                                                           ? data[i][j+5][k] : data[i][j+5-1][k] + data[i][j+5][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j+5][k] = (data[i][j+5][k] < data[i-1][j+5][k] + data[i][j+5][k-1])
                                                           ? data[i][j+5][k] : data[i-1][j+5][k] + data[i][j+5][k-1];
                                      }
                            }
                            {
                                if (i > 1 && j+6 > 1 && k > 1 && i < DEPTH - 2 && j+6 < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j+6][k];                   
                                        int candidate2 = data[i][k][j+6] + data[k][j+6][i];   
                                        int candidate3 = data[i-1][j+6][k] + data[i][j+6-1][k]; 
                                        int candidate4 = data[i][j+6][k-1] + data[i-1][j+6-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j+6][k] = new_val;
                                        if (i > 0 && j+6 > 0)
                                          data[i][j+6][k] = (data[i][j+6][k] < data[i-1][j+6][k] + data[i][j+6-1][k])
                                                           ? data[i][j+6][k] : data[i-1][j+6][k] + data[i][j+6-1][k];
                                        if (j+6 > 0 && k > 0)
                                          data[i][j+6][k] = (data[i][j+6][k] < data[i][j+6-1][k] + data[i][j+6][k-1])
                                                           ? data[i][j+6][k] : data[i][j+6-1][k] + data[i][j+6][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j+6][k] = (data[i][j+6][k] < data[i-1][j+6][k] + data[i][j+6][k-1])
                                                           ? data[i][j+6][k] : data[i-1][j+6][k] + data[i][j+6][k-1];
                                      }
                            }
                            {
                                if (i > 1 && j+7 > 1 && k > 1 && i < DEPTH - 2 && j+7 < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j+7][k];                   
                                        int candidate2 = data[i][k][j+7] + data[k][j+7][i];   
                                        int candidate3 = data[i-1][j+7][k] + data[i][j+7-1][k]; 
                                        int candidate4 = data[i][j+7][k-1] + data[i-1][j+7-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j+7][k] = new_val;
                                        if (i > 0 && j+7 > 0)
                                          data[i][j+7][k] = (data[i][j+7][k] < data[i-1][j+7][k] + data[i][j+7-1][k])
                                                           ? data[i][j+7][k] : data[i-1][j+7][k] + data[i][j+7-1][k];
                                        if (j+7 > 0 && k > 0)
                                          data[i][j+7][k] = (data[i][j+7][k] < data[i][j+7-1][k] + data[i][j+7][k-1])
                                                           ? data[i][j+7][k] : data[i][j+7-1][k] + data[i][j+7][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j+7][k] = (data[i][j+7][k] < data[i-1][j+7][k] + data[i][j+7][k-1])
                                                           ? data[i][j+7][k] : data[i-1][j+7][k] + data[i][j+7][k-1];
                                      }
                            }
                        }
                        if (j < j_end) {
                            for (; j < j_end; ++j) {
                                if (i > 1 && j > 1 && k > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i][j][k];                   
                                        int candidate2 = data[i][k][j] + data[k][j][i];   
                                        int candidate3 = data[i-1][j][k] + data[i][j-1][k]; 
                                        int candidate4 = data[i][j][k-1] + data[i-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k] = (data[i][j][k] < data[i-1][j][k] + data[i][j-1][k])
                                                           ? data[i][j][k] : data[i-1][j][k] + data[i][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i][j][k] = (data[i][j][k] < data[i][j-1][k] + data[i][j][k-1])
                                                           ? data[i][j][k] : data[i][j-1][k] + data[i][j][k-1];
                                        if (i > 0 && k > 0)
                                          data[i][j][k] = (data[i][j][k] < data[i-1][j][k] + data[i][j][k-1])
                                                           ? data[i][j][k] : data[i-1][j][k] + data[i][j][k-1];
                                      }
                            }
                        }
                    }
                }
            }
        }
    }
    end_cycles = rdtsc_serialized();
    printf("⏱️ Loop cycles: %llu\n", end_cycles - start_cycles);
    return 0;
}
