#include <stdio.h>
#include <stdint.h>

// Cycle counting function
static inline unsigned long long rdtsc_serialized(void) {
    unsigned int low, high;
    asm volatile ("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx");
    asm volatile ("rdtsc" : "=a" (low), "=d" (high));
    return ((unsigned long long)high << 32) | low;
}

#define TILE_SIZE 32
#define UNROLL 10
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
        for (int k_t = 0; k_t < WIDTH; k_t += TILE_SIZE) {
            for (int i_t = 0; i_t < DEPTH; i_t += TILE_SIZE) {
                int j_end = j_t + TILE_SIZE; if (j_end > HEIGHT) j_end = HEIGHT;
                for (int j = j_t; j < j_end; ++j) {
                    int k_end = k_t + TILE_SIZE; if (k_end > WIDTH) k_end = WIDTH;
                    for (int k = k_t; k < k_end; ++k) {
                        int i_end = i_t + TILE_SIZE; if (i_end > DEPTH) i_end = DEPTH;
                        int i = i_t;
                        for (; i + (UNROLL - 1) < i_end; i += 10) {
                            {
                                if (i+0 > 1 && j > 1 && k > 1 && i+0 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+0][j][k];                   
                                        int candidate2 = data[i+0][k][j] + data[k][j][i+0];   
                                        int candidate3 = data[i+0-1][j][k] + data[i+0][j-1][k]; 
                                        int candidate4 = data[i+0][j][k-1] + data[i+0-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+0][j][k] = new_val;
                                        if (i+0 > 0 && j > 0)
                                          data[i+0][j][k] = (data[i+0][j][k] < data[i+0-1][j][k] + data[i+0][j-1][k])
                                                           ? data[i+0][j][k] : data[i+0-1][j][k] + data[i+0][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+0][j][k] = (data[i+0][j][k] < data[i+0][j-1][k] + data[i+0][j][k-1])
                                                           ? data[i+0][j][k] : data[i+0][j-1][k] + data[i+0][j][k-1];
                                        if (i+0 > 0 && k > 0)
                                          data[i+0][j][k] = (data[i+0][j][k] < data[i+0-1][j][k] + data[i+0][j][k-1])
                                                           ? data[i+0][j][k] : data[i+0-1][j][k] + data[i+0][j][k-1];
                                      }
                            }
                            {
                                if (i+1 > 1 && j > 1 && k > 1 && i+1 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+1][j][k];                   
                                        int candidate2 = data[i+1][k][j] + data[k][j][i+1];   
                                        int candidate3 = data[i+1-1][j][k] + data[i+1][j-1][k]; 
                                        int candidate4 = data[i+1][j][k-1] + data[i+1-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+1][j][k] = new_val;
                                        if (i+1 > 0 && j > 0)
                                          data[i+1][j][k] = (data[i+1][j][k] < data[i+1-1][j][k] + data[i+1][j-1][k])
                                                           ? data[i+1][j][k] : data[i+1-1][j][k] + data[i+1][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+1][j][k] = (data[i+1][j][k] < data[i+1][j-1][k] + data[i+1][j][k-1])
                                                           ? data[i+1][j][k] : data[i+1][j-1][k] + data[i+1][j][k-1];
                                        if (i+1 > 0 && k > 0)
                                          data[i+1][j][k] = (data[i+1][j][k] < data[i+1-1][j][k] + data[i+1][j][k-1])
                                                           ? data[i+1][j][k] : data[i+1-1][j][k] + data[i+1][j][k-1];
                                      }
                            }
                            {
                                if (i+2 > 1 && j > 1 && k > 1 && i+2 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+2][j][k];                   
                                        int candidate2 = data[i+2][k][j] + data[k][j][i+2];   
                                        int candidate3 = data[i+2-1][j][k] + data[i+2][j-1][k]; 
                                        int candidate4 = data[i+2][j][k-1] + data[i+2-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+2][j][k] = new_val;
                                        if (i+2 > 0 && j > 0)
                                          data[i+2][j][k] = (data[i+2][j][k] < data[i+2-1][j][k] + data[i+2][j-1][k])
                                                           ? data[i+2][j][k] : data[i+2-1][j][k] + data[i+2][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+2][j][k] = (data[i+2][j][k] < data[i+2][j-1][k] + data[i+2][j][k-1])
                                                           ? data[i+2][j][k] : data[i+2][j-1][k] + data[i+2][j][k-1];
                                        if (i+2 > 0 && k > 0)
                                          data[i+2][j][k] = (data[i+2][j][k] < data[i+2-1][j][k] + data[i+2][j][k-1])
                                                           ? data[i+2][j][k] : data[i+2-1][j][k] + data[i+2][j][k-1];
                                      }
                            }
                            {
                                if (i+3 > 1 && j > 1 && k > 1 && i+3 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+3][j][k];                   
                                        int candidate2 = data[i+3][k][j] + data[k][j][i+3];   
                                        int candidate3 = data[i+3-1][j][k] + data[i+3][j-1][k]; 
                                        int candidate4 = data[i+3][j][k-1] + data[i+3-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+3][j][k] = new_val;
                                        if (i+3 > 0 && j > 0)
                                          data[i+3][j][k] = (data[i+3][j][k] < data[i+3-1][j][k] + data[i+3][j-1][k])
                                                           ? data[i+3][j][k] : data[i+3-1][j][k] + data[i+3][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+3][j][k] = (data[i+3][j][k] < data[i+3][j-1][k] + data[i+3][j][k-1])
                                                           ? data[i+3][j][k] : data[i+3][j-1][k] + data[i+3][j][k-1];
                                        if (i+3 > 0 && k > 0)
                                          data[i+3][j][k] = (data[i+3][j][k] < data[i+3-1][j][k] + data[i+3][j][k-1])
                                                           ? data[i+3][j][k] : data[i+3-1][j][k] + data[i+3][j][k-1];
                                      }
                            }
                            {
                                if (i+4 > 1 && j > 1 && k > 1 && i+4 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+4][j][k];                   
                                        int candidate2 = data[i+4][k][j] + data[k][j][i+4];   
                                        int candidate3 = data[i+4-1][j][k] + data[i+4][j-1][k]; 
                                        int candidate4 = data[i+4][j][k-1] + data[i+4-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+4][j][k] = new_val;
                                        if (i+4 > 0 && j > 0)
                                          data[i+4][j][k] = (data[i+4][j][k] < data[i+4-1][j][k] + data[i+4][j-1][k])
                                                           ? data[i+4][j][k] : data[i+4-1][j][k] + data[i+4][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+4][j][k] = (data[i+4][j][k] < data[i+4][j-1][k] + data[i+4][j][k-1])
                                                           ? data[i+4][j][k] : data[i+4][j-1][k] + data[i+4][j][k-1];
                                        if (i+4 > 0 && k > 0)
                                          data[i+4][j][k] = (data[i+4][j][k] < data[i+4-1][j][k] + data[i+4][j][k-1])
                                                           ? data[i+4][j][k] : data[i+4-1][j][k] + data[i+4][j][k-1];
                                      }
                            }
                            {
                                if (i+5 > 1 && j > 1 && k > 1 && i+5 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+5][j][k];                   
                                        int candidate2 = data[i+5][k][j] + data[k][j][i+5];   
                                        int candidate3 = data[i+5-1][j][k] + data[i+5][j-1][k]; 
                                        int candidate4 = data[i+5][j][k-1] + data[i+5-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+5][j][k] = new_val;
                                        if (i+5 > 0 && j > 0)
                                          data[i+5][j][k] = (data[i+5][j][k] < data[i+5-1][j][k] + data[i+5][j-1][k])
                                                           ? data[i+5][j][k] : data[i+5-1][j][k] + data[i+5][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+5][j][k] = (data[i+5][j][k] < data[i+5][j-1][k] + data[i+5][j][k-1])
                                                           ? data[i+5][j][k] : data[i+5][j-1][k] + data[i+5][j][k-1];
                                        if (i+5 > 0 && k > 0)
                                          data[i+5][j][k] = (data[i+5][j][k] < data[i+5-1][j][k] + data[i+5][j][k-1])
                                                           ? data[i+5][j][k] : data[i+5-1][j][k] + data[i+5][j][k-1];
                                      }
                            }
                            {
                                if (i+6 > 1 && j > 1 && k > 1 && i+6 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+6][j][k];                   
                                        int candidate2 = data[i+6][k][j] + data[k][j][i+6];   
                                        int candidate3 = data[i+6-1][j][k] + data[i+6][j-1][k]; 
                                        int candidate4 = data[i+6][j][k-1] + data[i+6-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+6][j][k] = new_val;
                                        if (i+6 > 0 && j > 0)
                                          data[i+6][j][k] = (data[i+6][j][k] < data[i+6-1][j][k] + data[i+6][j-1][k])
                                                           ? data[i+6][j][k] : data[i+6-1][j][k] + data[i+6][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+6][j][k] = (data[i+6][j][k] < data[i+6][j-1][k] + data[i+6][j][k-1])
                                                           ? data[i+6][j][k] : data[i+6][j-1][k] + data[i+6][j][k-1];
                                        if (i+6 > 0 && k > 0)
                                          data[i+6][j][k] = (data[i+6][j][k] < data[i+6-1][j][k] + data[i+6][j][k-1])
                                                           ? data[i+6][j][k] : data[i+6-1][j][k] + data[i+6][j][k-1];
                                      }
                            }
                            {
                                if (i+7 > 1 && j > 1 && k > 1 && i+7 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+7][j][k];                   
                                        int candidate2 = data[i+7][k][j] + data[k][j][i+7];   
                                        int candidate3 = data[i+7-1][j][k] + data[i+7][j-1][k]; 
                                        int candidate4 = data[i+7][j][k-1] + data[i+7-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+7][j][k] = new_val;
                                        if (i+7 > 0 && j > 0)
                                          data[i+7][j][k] = (data[i+7][j][k] < data[i+7-1][j][k] + data[i+7][j-1][k])
                                                           ? data[i+7][j][k] : data[i+7-1][j][k] + data[i+7][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+7][j][k] = (data[i+7][j][k] < data[i+7][j-1][k] + data[i+7][j][k-1])
                                                           ? data[i+7][j][k] : data[i+7][j-1][k] + data[i+7][j][k-1];
                                        if (i+7 > 0 && k > 0)
                                          data[i+7][j][k] = (data[i+7][j][k] < data[i+7-1][j][k] + data[i+7][j][k-1])
                                                           ? data[i+7][j][k] : data[i+7-1][j][k] + data[i+7][j][k-1];
                                      }
                            }
                            {
                                if (i+8 > 1 && j > 1 && k > 1 && i+8 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+8][j][k];                   
                                        int candidate2 = data[i+8][k][j] + data[k][j][i+8];   
                                        int candidate3 = data[i+8-1][j][k] + data[i+8][j-1][k]; 
                                        int candidate4 = data[i+8][j][k-1] + data[i+8-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+8][j][k] = new_val;
                                        if (i+8 > 0 && j > 0)
                                          data[i+8][j][k] = (data[i+8][j][k] < data[i+8-1][j][k] + data[i+8][j-1][k])
                                                           ? data[i+8][j][k] : data[i+8-1][j][k] + data[i+8][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+8][j][k] = (data[i+8][j][k] < data[i+8][j-1][k] + data[i+8][j][k-1])
                                                           ? data[i+8][j][k] : data[i+8][j-1][k] + data[i+8][j][k-1];
                                        if (i+8 > 0 && k > 0)
                                          data[i+8][j][k] = (data[i+8][j][k] < data[i+8-1][j][k] + data[i+8][j][k-1])
                                                           ? data[i+8][j][k] : data[i+8-1][j][k] + data[i+8][j][k-1];
                                      }
                            }
                            {
                                if (i+9 > 1 && j > 1 && k > 1 && i+9 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+9][j][k];                   
                                        int candidate2 = data[i+9][k][j] + data[k][j][i+9];   
                                        int candidate3 = data[i+9-1][j][k] + data[i+9][j-1][k]; 
                                        int candidate4 = data[i+9][j][k-1] + data[i+9-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+9][j][k] = new_val;
                                        if (i+9 > 0 && j > 0)
                                          data[i+9][j][k] = (data[i+9][j][k] < data[i+9-1][j][k] + data[i+9][j-1][k])
                                                           ? data[i+9][j][k] : data[i+9-1][j][k] + data[i+9][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+9][j][k] = (data[i+9][j][k] < data[i+9][j-1][k] + data[i+9][j][k-1])
                                                           ? data[i+9][j][k] : data[i+9][j-1][k] + data[i+9][j][k-1];
                                        if (i+9 > 0 && k > 0)
                                          data[i+9][j][k] = (data[i+9][j][k] < data[i+9-1][j][k] + data[i+9][j][k-1])
                                                           ? data[i+9][j][k] : data[i+9-1][j][k] + data[i+9][j][k-1];
                                      }
                            }
                        }
                        if (i < i_end) {
                            for (; i < i_end; ++i) {
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
