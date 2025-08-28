#include <stdio.h>
#include <stdint.h>

// Cycle counting function
static inline unsigned long long rdtsc_serialized(void) {
    unsigned int low, high;
    asm volatile ("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx");
    asm volatile ("rdtsc" : "=a" (low), "=d" (high));
    return ((unsigned long long)high << 32) | low;
}

#define TILE_SIZE 128
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
    for (int j_t = 0; j_t < HEIGHT; j_t += TILE_SIZE) {
        for (int i_t = 0; i_t < DEPTH; i_t += TILE_SIZE) {
            int j_end = j_t + TILE_SIZE; if (j_end > HEIGHT) j_end = HEIGHT;
            for (int j = j_t; j < j_end; ++j) {
                int i_end = i_t + TILE_SIZE; if (i_end > DEPTH) i_end = DEPTH;
                for (int i = i_t; i < i_end; ++i) {
                    for (int k_t = 0; k_t < WIDTH; k_t += TILE_SIZE) {
                        int k_end = k_t + TILE_SIZE; if (k_end > WIDTH) k_end = WIDTH;
                        int k = k_t;
                        for (; k + (UNROLL - 1) < k_end; k += 8) {
                            {
                                if (i > 1 && j > 1 && k+0 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+0 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+0];                   
                                        int candidate2 = data[i][k+0][j] + data[k+0][j][i];   
                                        int candidate3 = data[i-1][j][k+0] + data[i][j-1][k+0]; 
                                        int candidate4 = data[i][j][k+0-1] + data[i-1][j-1][k+0-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+0] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+0] = (data[i][j][k+0] < data[i-1][j][k+0] + data[i][j-1][k+0])
                                                           ? data[i][j][k+0] : data[i-1][j][k+0] + data[i][j-1][k+0];
                                        if (j > 0 && k+0 > 0)
                                          data[i][j][k+0] = (data[i][j][k+0] < data[i][j-1][k+0] + data[i][j][k+0-1])
                                                           ? data[i][j][k+0] : data[i][j-1][k+0] + data[i][j][k+0-1];
                                        if (i > 0 && k+0 > 0)
                                          data[i][j][k+0] = (data[i][j][k+0] < data[i-1][j][k+0] + data[i][j][k+0-1])
                                                           ? data[i][j][k+0] : data[i-1][j][k+0] + data[i][j][k+0-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+1 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+1 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+1];                   
                                        int candidate2 = data[i][k+1][j] + data[k+1][j][i];   
                                        int candidate3 = data[i-1][j][k+1] + data[i][j-1][k+1]; 
                                        int candidate4 = data[i][j][k+1-1] + data[i-1][j-1][k+1-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+1] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+1] = (data[i][j][k+1] < data[i-1][j][k+1] + data[i][j-1][k+1])
                                                           ? data[i][j][k+1] : data[i-1][j][k+1] + data[i][j-1][k+1];
                                        if (j > 0 && k+1 > 0)
                                          data[i][j][k+1] = (data[i][j][k+1] < data[i][j-1][k+1] + data[i][j][k+1-1])
                                                           ? data[i][j][k+1] : data[i][j-1][k+1] + data[i][j][k+1-1];
                                        if (i > 0 && k+1 > 0)
                                          data[i][j][k+1] = (data[i][j][k+1] < data[i-1][j][k+1] + data[i][j][k+1-1])
                                                           ? data[i][j][k+1] : data[i-1][j][k+1] + data[i][j][k+1-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+2 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+2 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+2];                   
                                        int candidate2 = data[i][k+2][j] + data[k+2][j][i];   
                                        int candidate3 = data[i-1][j][k+2] + data[i][j-1][k+2]; 
                                        int candidate4 = data[i][j][k+2-1] + data[i-1][j-1][k+2-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+2] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+2] = (data[i][j][k+2] < data[i-1][j][k+2] + data[i][j-1][k+2])
                                                           ? data[i][j][k+2] : data[i-1][j][k+2] + data[i][j-1][k+2];
                                        if (j > 0 && k+2 > 0)
                                          data[i][j][k+2] = (data[i][j][k+2] < data[i][j-1][k+2] + data[i][j][k+2-1])
                                                           ? data[i][j][k+2] : data[i][j-1][k+2] + data[i][j][k+2-1];
                                        if (i > 0 && k+2 > 0)
                                          data[i][j][k+2] = (data[i][j][k+2] < data[i-1][j][k+2] + data[i][j][k+2-1])
                                                           ? data[i][j][k+2] : data[i-1][j][k+2] + data[i][j][k+2-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+3 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+3 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+3];                   
                                        int candidate2 = data[i][k+3][j] + data[k+3][j][i];   
                                        int candidate3 = data[i-1][j][k+3] + data[i][j-1][k+3]; 
                                        int candidate4 = data[i][j][k+3-1] + data[i-1][j-1][k+3-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+3] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+3] = (data[i][j][k+3] < data[i-1][j][k+3] + data[i][j-1][k+3])
                                                           ? data[i][j][k+3] : data[i-1][j][k+3] + data[i][j-1][k+3];
                                        if (j > 0 && k+3 > 0)
                                          data[i][j][k+3] = (data[i][j][k+3] < data[i][j-1][k+3] + data[i][j][k+3-1])
                                                           ? data[i][j][k+3] : data[i][j-1][k+3] + data[i][j][k+3-1];
                                        if (i > 0 && k+3 > 0)
                                          data[i][j][k+3] = (data[i][j][k+3] < data[i-1][j][k+3] + data[i][j][k+3-1])
                                                           ? data[i][j][k+3] : data[i-1][j][k+3] + data[i][j][k+3-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+4 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+4 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+4];                   
                                        int candidate2 = data[i][k+4][j] + data[k+4][j][i];   
                                        int candidate3 = data[i-1][j][k+4] + data[i][j-1][k+4]; 
                                        int candidate4 = data[i][j][k+4-1] + data[i-1][j-1][k+4-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+4] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+4] = (data[i][j][k+4] < data[i-1][j][k+4] + data[i][j-1][k+4])
                                                           ? data[i][j][k+4] : data[i-1][j][k+4] + data[i][j-1][k+4];
                                        if (j > 0 && k+4 > 0)
                                          data[i][j][k+4] = (data[i][j][k+4] < data[i][j-1][k+4] + data[i][j][k+4-1])
                                                           ? data[i][j][k+4] : data[i][j-1][k+4] + data[i][j][k+4-1];
                                        if (i > 0 && k+4 > 0)
                                          data[i][j][k+4] = (data[i][j][k+4] < data[i-1][j][k+4] + data[i][j][k+4-1])
                                                           ? data[i][j][k+4] : data[i-1][j][k+4] + data[i][j][k+4-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+5 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+5 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+5];                   
                                        int candidate2 = data[i][k+5][j] + data[k+5][j][i];   
                                        int candidate3 = data[i-1][j][k+5] + data[i][j-1][k+5]; 
                                        int candidate4 = data[i][j][k+5-1] + data[i-1][j-1][k+5-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+5] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+5] = (data[i][j][k+5] < data[i-1][j][k+5] + data[i][j-1][k+5])
                                                           ? data[i][j][k+5] : data[i-1][j][k+5] + data[i][j-1][k+5];
                                        if (j > 0 && k+5 > 0)
                                          data[i][j][k+5] = (data[i][j][k+5] < data[i][j-1][k+5] + data[i][j][k+5-1])
                                                           ? data[i][j][k+5] : data[i][j-1][k+5] + data[i][j][k+5-1];
                                        if (i > 0 && k+5 > 0)
                                          data[i][j][k+5] = (data[i][j][k+5] < data[i-1][j][k+5] + data[i][j][k+5-1])
                                                           ? data[i][j][k+5] : data[i-1][j][k+5] + data[i][j][k+5-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+6 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+6 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+6];                   
                                        int candidate2 = data[i][k+6][j] + data[k+6][j][i];   
                                        int candidate3 = data[i-1][j][k+6] + data[i][j-1][k+6]; 
                                        int candidate4 = data[i][j][k+6-1] + data[i-1][j-1][k+6-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+6] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+6] = (data[i][j][k+6] < data[i-1][j][k+6] + data[i][j-1][k+6])
                                                           ? data[i][j][k+6] : data[i-1][j][k+6] + data[i][j-1][k+6];
                                        if (j > 0 && k+6 > 0)
                                          data[i][j][k+6] = (data[i][j][k+6] < data[i][j-1][k+6] + data[i][j][k+6-1])
                                                           ? data[i][j][k+6] : data[i][j-1][k+6] + data[i][j][k+6-1];
                                        if (i > 0 && k+6 > 0)
                                          data[i][j][k+6] = (data[i][j][k+6] < data[i-1][j][k+6] + data[i][j][k+6-1])
                                                           ? data[i][j][k+6] : data[i-1][j][k+6] + data[i][j][k+6-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+7 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+7 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+7];                   
                                        int candidate2 = data[i][k+7][j] + data[k+7][j][i];   
                                        int candidate3 = data[i-1][j][k+7] + data[i][j-1][k+7]; 
                                        int candidate4 = data[i][j][k+7-1] + data[i-1][j-1][k+7-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+7] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+7] = (data[i][j][k+7] < data[i-1][j][k+7] + data[i][j-1][k+7])
                                                           ? data[i][j][k+7] : data[i-1][j][k+7] + data[i][j-1][k+7];
                                        if (j > 0 && k+7 > 0)
                                          data[i][j][k+7] = (data[i][j][k+7] < data[i][j-1][k+7] + data[i][j][k+7-1])
                                                           ? data[i][j][k+7] : data[i][j-1][k+7] + data[i][j][k+7-1];
                                        if (i > 0 && k+7 > 0)
                                          data[i][j][k+7] = (data[i][j][k+7] < data[i-1][j][k+7] + data[i][j][k+7-1])
                                                           ? data[i][j][k+7] : data[i-1][j][k+7] + data[i][j][k+7-1];
                                      }
                            }
                        }
                        if (k < k_end) {
                            for (; k < k_end; ++k) {
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
