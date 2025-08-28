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
#define UNROLL 20
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
                for (int k_t = 0; k_t < WIDTH; k_t += TILE_SIZE) {
                    int i_end = i_t + TILE_SIZE; if (i_end > DEPTH) i_end = DEPTH;
                    for (int i = i_t; i < i_end; ++i) {
                        int k_end = k_t + TILE_SIZE; if (k_end > WIDTH) k_end = WIDTH;
                        int k = k_t;
                        for (; k + (UNROLL - 1) < k_end; k += 20) {
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
                            {
                                if (i > 1 && j > 1 && k+8 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+8 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+8];                   
                                        int candidate2 = data[i][k+8][j] + data[k+8][j][i];   
                                        int candidate3 = data[i-1][j][k+8] + data[i][j-1][k+8]; 
                                        int candidate4 = data[i][j][k+8-1] + data[i-1][j-1][k+8-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+8] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+8] = (data[i][j][k+8] < data[i-1][j][k+8] + data[i][j-1][k+8])
                                                           ? data[i][j][k+8] : data[i-1][j][k+8] + data[i][j-1][k+8];
                                        if (j > 0 && k+8 > 0)
                                          data[i][j][k+8] = (data[i][j][k+8] < data[i][j-1][k+8] + data[i][j][k+8-1])
                                                           ? data[i][j][k+8] : data[i][j-1][k+8] + data[i][j][k+8-1];
                                        if (i > 0 && k+8 > 0)
                                          data[i][j][k+8] = (data[i][j][k+8] < data[i-1][j][k+8] + data[i][j][k+8-1])
                                                           ? data[i][j][k+8] : data[i-1][j][k+8] + data[i][j][k+8-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+9 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+9 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+9];                   
                                        int candidate2 = data[i][k+9][j] + data[k+9][j][i];   
                                        int candidate3 = data[i-1][j][k+9] + data[i][j-1][k+9]; 
                                        int candidate4 = data[i][j][k+9-1] + data[i-1][j-1][k+9-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+9] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+9] = (data[i][j][k+9] < data[i-1][j][k+9] + data[i][j-1][k+9])
                                                           ? data[i][j][k+9] : data[i-1][j][k+9] + data[i][j-1][k+9];
                                        if (j > 0 && k+9 > 0)
                                          data[i][j][k+9] = (data[i][j][k+9] < data[i][j-1][k+9] + data[i][j][k+9-1])
                                                           ? data[i][j][k+9] : data[i][j-1][k+9] + data[i][j][k+9-1];
                                        if (i > 0 && k+9 > 0)
                                          data[i][j][k+9] = (data[i][j][k+9] < data[i-1][j][k+9] + data[i][j][k+9-1])
                                                           ? data[i][j][k+9] : data[i-1][j][k+9] + data[i][j][k+9-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+10 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+10 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+10];                   
                                        int candidate2 = data[i][k+10][j] + data[k+10][j][i];   
                                        int candidate3 = data[i-1][j][k+10] + data[i][j-1][k+10]; 
                                        int candidate4 = data[i][j][k+10-1] + data[i-1][j-1][k+10-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+10] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+10] = (data[i][j][k+10] < data[i-1][j][k+10] + data[i][j-1][k+10])
                                                           ? data[i][j][k+10] : data[i-1][j][k+10] + data[i][j-1][k+10];
                                        if (j > 0 && k+10 > 0)
                                          data[i][j][k+10] = (data[i][j][k+10] < data[i][j-1][k+10] + data[i][j][k+10-1])
                                                           ? data[i][j][k+10] : data[i][j-1][k+10] + data[i][j][k+10-1];
                                        if (i > 0 && k+10 > 0)
                                          data[i][j][k+10] = (data[i][j][k+10] < data[i-1][j][k+10] + data[i][j][k+10-1])
                                                           ? data[i][j][k+10] : data[i-1][j][k+10] + data[i][j][k+10-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+11 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+11 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+11];                   
                                        int candidate2 = data[i][k+11][j] + data[k+11][j][i];   
                                        int candidate3 = data[i-1][j][k+11] + data[i][j-1][k+11]; 
                                        int candidate4 = data[i][j][k+11-1] + data[i-1][j-1][k+11-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+11] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+11] = (data[i][j][k+11] < data[i-1][j][k+11] + data[i][j-1][k+11])
                                                           ? data[i][j][k+11] : data[i-1][j][k+11] + data[i][j-1][k+11];
                                        if (j > 0 && k+11 > 0)
                                          data[i][j][k+11] = (data[i][j][k+11] < data[i][j-1][k+11] + data[i][j][k+11-1])
                                                           ? data[i][j][k+11] : data[i][j-1][k+11] + data[i][j][k+11-1];
                                        if (i > 0 && k+11 > 0)
                                          data[i][j][k+11] = (data[i][j][k+11] < data[i-1][j][k+11] + data[i][j][k+11-1])
                                                           ? data[i][j][k+11] : data[i-1][j][k+11] + data[i][j][k+11-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+12 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+12 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+12];                   
                                        int candidate2 = data[i][k+12][j] + data[k+12][j][i];   
                                        int candidate3 = data[i-1][j][k+12] + data[i][j-1][k+12]; 
                                        int candidate4 = data[i][j][k+12-1] + data[i-1][j-1][k+12-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+12] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+12] = (data[i][j][k+12] < data[i-1][j][k+12] + data[i][j-1][k+12])
                                                           ? data[i][j][k+12] : data[i-1][j][k+12] + data[i][j-1][k+12];
                                        if (j > 0 && k+12 > 0)
                                          data[i][j][k+12] = (data[i][j][k+12] < data[i][j-1][k+12] + data[i][j][k+12-1])
                                                           ? data[i][j][k+12] : data[i][j-1][k+12] + data[i][j][k+12-1];
                                        if (i > 0 && k+12 > 0)
                                          data[i][j][k+12] = (data[i][j][k+12] < data[i-1][j][k+12] + data[i][j][k+12-1])
                                                           ? data[i][j][k+12] : data[i-1][j][k+12] + data[i][j][k+12-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+13 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+13 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+13];                   
                                        int candidate2 = data[i][k+13][j] + data[k+13][j][i];   
                                        int candidate3 = data[i-1][j][k+13] + data[i][j-1][k+13]; 
                                        int candidate4 = data[i][j][k+13-1] + data[i-1][j-1][k+13-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+13] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+13] = (data[i][j][k+13] < data[i-1][j][k+13] + data[i][j-1][k+13])
                                                           ? data[i][j][k+13] : data[i-1][j][k+13] + data[i][j-1][k+13];
                                        if (j > 0 && k+13 > 0)
                                          data[i][j][k+13] = (data[i][j][k+13] < data[i][j-1][k+13] + data[i][j][k+13-1])
                                                           ? data[i][j][k+13] : data[i][j-1][k+13] + data[i][j][k+13-1];
                                        if (i > 0 && k+13 > 0)
                                          data[i][j][k+13] = (data[i][j][k+13] < data[i-1][j][k+13] + data[i][j][k+13-1])
                                                           ? data[i][j][k+13] : data[i-1][j][k+13] + data[i][j][k+13-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+14 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+14 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+14];                   
                                        int candidate2 = data[i][k+14][j] + data[k+14][j][i];   
                                        int candidate3 = data[i-1][j][k+14] + data[i][j-1][k+14]; 
                                        int candidate4 = data[i][j][k+14-1] + data[i-1][j-1][k+14-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+14] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+14] = (data[i][j][k+14] < data[i-1][j][k+14] + data[i][j-1][k+14])
                                                           ? data[i][j][k+14] : data[i-1][j][k+14] + data[i][j-1][k+14];
                                        if (j > 0 && k+14 > 0)
                                          data[i][j][k+14] = (data[i][j][k+14] < data[i][j-1][k+14] + data[i][j][k+14-1])
                                                           ? data[i][j][k+14] : data[i][j-1][k+14] + data[i][j][k+14-1];
                                        if (i > 0 && k+14 > 0)
                                          data[i][j][k+14] = (data[i][j][k+14] < data[i-1][j][k+14] + data[i][j][k+14-1])
                                                           ? data[i][j][k+14] : data[i-1][j][k+14] + data[i][j][k+14-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+15 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+15 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+15];                   
                                        int candidate2 = data[i][k+15][j] + data[k+15][j][i];   
                                        int candidate3 = data[i-1][j][k+15] + data[i][j-1][k+15]; 
                                        int candidate4 = data[i][j][k+15-1] + data[i-1][j-1][k+15-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+15] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+15] = (data[i][j][k+15] < data[i-1][j][k+15] + data[i][j-1][k+15])
                                                           ? data[i][j][k+15] : data[i-1][j][k+15] + data[i][j-1][k+15];
                                        if (j > 0 && k+15 > 0)
                                          data[i][j][k+15] = (data[i][j][k+15] < data[i][j-1][k+15] + data[i][j][k+15-1])
                                                           ? data[i][j][k+15] : data[i][j-1][k+15] + data[i][j][k+15-1];
                                        if (i > 0 && k+15 > 0)
                                          data[i][j][k+15] = (data[i][j][k+15] < data[i-1][j][k+15] + data[i][j][k+15-1])
                                                           ? data[i][j][k+15] : data[i-1][j][k+15] + data[i][j][k+15-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+16 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+16 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+16];                   
                                        int candidate2 = data[i][k+16][j] + data[k+16][j][i];   
                                        int candidate3 = data[i-1][j][k+16] + data[i][j-1][k+16]; 
                                        int candidate4 = data[i][j][k+16-1] + data[i-1][j-1][k+16-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+16] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+16] = (data[i][j][k+16] < data[i-1][j][k+16] + data[i][j-1][k+16])
                                                           ? data[i][j][k+16] : data[i-1][j][k+16] + data[i][j-1][k+16];
                                        if (j > 0 && k+16 > 0)
                                          data[i][j][k+16] = (data[i][j][k+16] < data[i][j-1][k+16] + data[i][j][k+16-1])
                                                           ? data[i][j][k+16] : data[i][j-1][k+16] + data[i][j][k+16-1];
                                        if (i > 0 && k+16 > 0)
                                          data[i][j][k+16] = (data[i][j][k+16] < data[i-1][j][k+16] + data[i][j][k+16-1])
                                                           ? data[i][j][k+16] : data[i-1][j][k+16] + data[i][j][k+16-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+17 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+17 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+17];                   
                                        int candidate2 = data[i][k+17][j] + data[k+17][j][i];   
                                        int candidate3 = data[i-1][j][k+17] + data[i][j-1][k+17]; 
                                        int candidate4 = data[i][j][k+17-1] + data[i-1][j-1][k+17-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+17] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+17] = (data[i][j][k+17] < data[i-1][j][k+17] + data[i][j-1][k+17])
                                                           ? data[i][j][k+17] : data[i-1][j][k+17] + data[i][j-1][k+17];
                                        if (j > 0 && k+17 > 0)
                                          data[i][j][k+17] = (data[i][j][k+17] < data[i][j-1][k+17] + data[i][j][k+17-1])
                                                           ? data[i][j][k+17] : data[i][j-1][k+17] + data[i][j][k+17-1];
                                        if (i > 0 && k+17 > 0)
                                          data[i][j][k+17] = (data[i][j][k+17] < data[i-1][j][k+17] + data[i][j][k+17-1])
                                                           ? data[i][j][k+17] : data[i-1][j][k+17] + data[i][j][k+17-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+18 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+18 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+18];                   
                                        int candidate2 = data[i][k+18][j] + data[k+18][j][i];   
                                        int candidate3 = data[i-1][j][k+18] + data[i][j-1][k+18]; 
                                        int candidate4 = data[i][j][k+18-1] + data[i-1][j-1][k+18-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+18] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+18] = (data[i][j][k+18] < data[i-1][j][k+18] + data[i][j-1][k+18])
                                                           ? data[i][j][k+18] : data[i-1][j][k+18] + data[i][j-1][k+18];
                                        if (j > 0 && k+18 > 0)
                                          data[i][j][k+18] = (data[i][j][k+18] < data[i][j-1][k+18] + data[i][j][k+18-1])
                                                           ? data[i][j][k+18] : data[i][j-1][k+18] + data[i][j][k+18-1];
                                        if (i > 0 && k+18 > 0)
                                          data[i][j][k+18] = (data[i][j][k+18] < data[i-1][j][k+18] + data[i][j][k+18-1])
                                                           ? data[i][j][k+18] : data[i-1][j][k+18] + data[i][j][k+18-1];
                                      }
                            }
                            {
                                if (i > 1 && j > 1 && k+19 > 1 && i < DEPTH - 2 && j < HEIGHT - 2 && k+19 < WIDTH - 2) {
                                        int candidate1 = data[i][j][k+19];                   
                                        int candidate2 = data[i][k+19][j] + data[k+19][j][i];   
                                        int candidate3 = data[i-1][j][k+19] + data[i][j-1][k+19]; 
                                        int candidate4 = data[i][j][k+19-1] + data[i-1][j-1][k+19-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i][j][k+19] = new_val;
                                        if (i > 0 && j > 0)
                                          data[i][j][k+19] = (data[i][j][k+19] < data[i-1][j][k+19] + data[i][j-1][k+19])
                                                           ? data[i][j][k+19] : data[i-1][j][k+19] + data[i][j-1][k+19];
                                        if (j > 0 && k+19 > 0)
                                          data[i][j][k+19] = (data[i][j][k+19] < data[i][j-1][k+19] + data[i][j][k+19-1])
                                                           ? data[i][j][k+19] : data[i][j-1][k+19] + data[i][j][k+19-1];
                                        if (i > 0 && k+19 > 0)
                                          data[i][j][k+19] = (data[i][j][k+19] < data[i-1][j][k+19] + data[i][j][k+19-1])
                                                           ? data[i][j][k+19] : data[i-1][j][k+19] + data[i][j][k+19-1];
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
