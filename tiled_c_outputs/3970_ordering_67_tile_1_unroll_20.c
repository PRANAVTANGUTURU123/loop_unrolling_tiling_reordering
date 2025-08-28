#include <stdio.h>
#include <stdint.h>

// Cycle counting function
static inline unsigned long long rdtsc_serialized(void) {
    unsigned int low, high;
    asm volatile ("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx");
    asm volatile ("rdtsc" : "=a" (low), "=d" (high));
    return ((unsigned long long)high << 32) | low;
}

#define TILE_SIZE 8
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
    for (int k_t = 0; k_t < WIDTH; k_t += TILE_SIZE) {
        for (int i_t = 0; i_t < DEPTH; i_t += TILE_SIZE) {
            for (int j_t = 0; j_t < HEIGHT; j_t += TILE_SIZE) {
                int j_end = j_t + TILE_SIZE; if (j_end > HEIGHT) j_end = HEIGHT;
                for (int j = j_t; j < j_end; ++j) {
                    int k_end = k_t + TILE_SIZE; if (k_end > WIDTH) k_end = WIDTH;
                    for (int k = k_t; k < k_end; ++k) {
                        int i_end = i_t + TILE_SIZE; if (i_end > DEPTH) i_end = DEPTH;
                        int i = i_t;
                        for (; i + (UNROLL - 1) < i_end; i += 20) {
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
                            {
                                if (i+10 > 1 && j > 1 && k > 1 && i+10 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+10][j][k];                   
                                        int candidate2 = data[i+10][k][j] + data[k][j][i+10];   
                                        int candidate3 = data[i+10-1][j][k] + data[i+10][j-1][k]; 
                                        int candidate4 = data[i+10][j][k-1] + data[i+10-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+10][j][k] = new_val;
                                        if (i+10 > 0 && j > 0)
                                          data[i+10][j][k] = (data[i+10][j][k] < data[i+10-1][j][k] + data[i+10][j-1][k])
                                                           ? data[i+10][j][k] : data[i+10-1][j][k] + data[i+10][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+10][j][k] = (data[i+10][j][k] < data[i+10][j-1][k] + data[i+10][j][k-1])
                                                           ? data[i+10][j][k] : data[i+10][j-1][k] + data[i+10][j][k-1];
                                        if (i+10 > 0 && k > 0)
                                          data[i+10][j][k] = (data[i+10][j][k] < data[i+10-1][j][k] + data[i+10][j][k-1])
                                                           ? data[i+10][j][k] : data[i+10-1][j][k] + data[i+10][j][k-1];
                                      }
                            }
                            {
                                if (i+11 > 1 && j > 1 && k > 1 && i+11 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+11][j][k];                   
                                        int candidate2 = data[i+11][k][j] + data[k][j][i+11];   
                                        int candidate3 = data[i+11-1][j][k] + data[i+11][j-1][k]; 
                                        int candidate4 = data[i+11][j][k-1] + data[i+11-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+11][j][k] = new_val;
                                        if (i+11 > 0 && j > 0)
                                          data[i+11][j][k] = (data[i+11][j][k] < data[i+11-1][j][k] + data[i+11][j-1][k])
                                                           ? data[i+11][j][k] : data[i+11-1][j][k] + data[i+11][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+11][j][k] = (data[i+11][j][k] < data[i+11][j-1][k] + data[i+11][j][k-1])
                                                           ? data[i+11][j][k] : data[i+11][j-1][k] + data[i+11][j][k-1];
                                        if (i+11 > 0 && k > 0)
                                          data[i+11][j][k] = (data[i+11][j][k] < data[i+11-1][j][k] + data[i+11][j][k-1])
                                                           ? data[i+11][j][k] : data[i+11-1][j][k] + data[i+11][j][k-1];
                                      }
                            }
                            {
                                if (i+12 > 1 && j > 1 && k > 1 && i+12 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+12][j][k];                   
                                        int candidate2 = data[i+12][k][j] + data[k][j][i+12];   
                                        int candidate3 = data[i+12-1][j][k] + data[i+12][j-1][k]; 
                                        int candidate4 = data[i+12][j][k-1] + data[i+12-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+12][j][k] = new_val;
                                        if (i+12 > 0 && j > 0)
                                          data[i+12][j][k] = (data[i+12][j][k] < data[i+12-1][j][k] + data[i+12][j-1][k])
                                                           ? data[i+12][j][k] : data[i+12-1][j][k] + data[i+12][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+12][j][k] = (data[i+12][j][k] < data[i+12][j-1][k] + data[i+12][j][k-1])
                                                           ? data[i+12][j][k] : data[i+12][j-1][k] + data[i+12][j][k-1];
                                        if (i+12 > 0 && k > 0)
                                          data[i+12][j][k] = (data[i+12][j][k] < data[i+12-1][j][k] + data[i+12][j][k-1])
                                                           ? data[i+12][j][k] : data[i+12-1][j][k] + data[i+12][j][k-1];
                                      }
                            }
                            {
                                if (i+13 > 1 && j > 1 && k > 1 && i+13 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+13][j][k];                   
                                        int candidate2 = data[i+13][k][j] + data[k][j][i+13];   
                                        int candidate3 = data[i+13-1][j][k] + data[i+13][j-1][k]; 
                                        int candidate4 = data[i+13][j][k-1] + data[i+13-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+13][j][k] = new_val;
                                        if (i+13 > 0 && j > 0)
                                          data[i+13][j][k] = (data[i+13][j][k] < data[i+13-1][j][k] + data[i+13][j-1][k])
                                                           ? data[i+13][j][k] : data[i+13-1][j][k] + data[i+13][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+13][j][k] = (data[i+13][j][k] < data[i+13][j-1][k] + data[i+13][j][k-1])
                                                           ? data[i+13][j][k] : data[i+13][j-1][k] + data[i+13][j][k-1];
                                        if (i+13 > 0 && k > 0)
                                          data[i+13][j][k] = (data[i+13][j][k] < data[i+13-1][j][k] + data[i+13][j][k-1])
                                                           ? data[i+13][j][k] : data[i+13-1][j][k] + data[i+13][j][k-1];
                                      }
                            }
                            {
                                if (i+14 > 1 && j > 1 && k > 1 && i+14 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+14][j][k];                   
                                        int candidate2 = data[i+14][k][j] + data[k][j][i+14];   
                                        int candidate3 = data[i+14-1][j][k] + data[i+14][j-1][k]; 
                                        int candidate4 = data[i+14][j][k-1] + data[i+14-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+14][j][k] = new_val;
                                        if (i+14 > 0 && j > 0)
                                          data[i+14][j][k] = (data[i+14][j][k] < data[i+14-1][j][k] + data[i+14][j-1][k])
                                                           ? data[i+14][j][k] : data[i+14-1][j][k] + data[i+14][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+14][j][k] = (data[i+14][j][k] < data[i+14][j-1][k] + data[i+14][j][k-1])
                                                           ? data[i+14][j][k] : data[i+14][j-1][k] + data[i+14][j][k-1];
                                        if (i+14 > 0 && k > 0)
                                          data[i+14][j][k] = (data[i+14][j][k] < data[i+14-1][j][k] + data[i+14][j][k-1])
                                                           ? data[i+14][j][k] : data[i+14-1][j][k] + data[i+14][j][k-1];
                                      }
                            }
                            {
                                if (i+15 > 1 && j > 1 && k > 1 && i+15 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+15][j][k];                   
                                        int candidate2 = data[i+15][k][j] + data[k][j][i+15];   
                                        int candidate3 = data[i+15-1][j][k] + data[i+15][j-1][k]; 
                                        int candidate4 = data[i+15][j][k-1] + data[i+15-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+15][j][k] = new_val;
                                        if (i+15 > 0 && j > 0)
                                          data[i+15][j][k] = (data[i+15][j][k] < data[i+15-1][j][k] + data[i+15][j-1][k])
                                                           ? data[i+15][j][k] : data[i+15-1][j][k] + data[i+15][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+15][j][k] = (data[i+15][j][k] < data[i+15][j-1][k] + data[i+15][j][k-1])
                                                           ? data[i+15][j][k] : data[i+15][j-1][k] + data[i+15][j][k-1];
                                        if (i+15 > 0 && k > 0)
                                          data[i+15][j][k] = (data[i+15][j][k] < data[i+15-1][j][k] + data[i+15][j][k-1])
                                                           ? data[i+15][j][k] : data[i+15-1][j][k] + data[i+15][j][k-1];
                                      }
                            }
                            {
                                if (i+16 > 1 && j > 1 && k > 1 && i+16 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+16][j][k];                   
                                        int candidate2 = data[i+16][k][j] + data[k][j][i+16];   
                                        int candidate3 = data[i+16-1][j][k] + data[i+16][j-1][k]; 
                                        int candidate4 = data[i+16][j][k-1] + data[i+16-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+16][j][k] = new_val;
                                        if (i+16 > 0 && j > 0)
                                          data[i+16][j][k] = (data[i+16][j][k] < data[i+16-1][j][k] + data[i+16][j-1][k])
                                                           ? data[i+16][j][k] : data[i+16-1][j][k] + data[i+16][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+16][j][k] = (data[i+16][j][k] < data[i+16][j-1][k] + data[i+16][j][k-1])
                                                           ? data[i+16][j][k] : data[i+16][j-1][k] + data[i+16][j][k-1];
                                        if (i+16 > 0 && k > 0)
                                          data[i+16][j][k] = (data[i+16][j][k] < data[i+16-1][j][k] + data[i+16][j][k-1])
                                                           ? data[i+16][j][k] : data[i+16-1][j][k] + data[i+16][j][k-1];
                                      }
                            }
                            {
                                if (i+17 > 1 && j > 1 && k > 1 && i+17 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+17][j][k];                   
                                        int candidate2 = data[i+17][k][j] + data[k][j][i+17];   
                                        int candidate3 = data[i+17-1][j][k] + data[i+17][j-1][k]; 
                                        int candidate4 = data[i+17][j][k-1] + data[i+17-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+17][j][k] = new_val;
                                        if (i+17 > 0 && j > 0)
                                          data[i+17][j][k] = (data[i+17][j][k] < data[i+17-1][j][k] + data[i+17][j-1][k])
                                                           ? data[i+17][j][k] : data[i+17-1][j][k] + data[i+17][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+17][j][k] = (data[i+17][j][k] < data[i+17][j-1][k] + data[i+17][j][k-1])
                                                           ? data[i+17][j][k] : data[i+17][j-1][k] + data[i+17][j][k-1];
                                        if (i+17 > 0 && k > 0)
                                          data[i+17][j][k] = (data[i+17][j][k] < data[i+17-1][j][k] + data[i+17][j][k-1])
                                                           ? data[i+17][j][k] : data[i+17-1][j][k] + data[i+17][j][k-1];
                                      }
                            }
                            {
                                if (i+18 > 1 && j > 1 && k > 1 && i+18 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+18][j][k];                   
                                        int candidate2 = data[i+18][k][j] + data[k][j][i+18];   
                                        int candidate3 = data[i+18-1][j][k] + data[i+18][j-1][k]; 
                                        int candidate4 = data[i+18][j][k-1] + data[i+18-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+18][j][k] = new_val;
                                        if (i+18 > 0 && j > 0)
                                          data[i+18][j][k] = (data[i+18][j][k] < data[i+18-1][j][k] + data[i+18][j-1][k])
                                                           ? data[i+18][j][k] : data[i+18-1][j][k] + data[i+18][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+18][j][k] = (data[i+18][j][k] < data[i+18][j-1][k] + data[i+18][j][k-1])
                                                           ? data[i+18][j][k] : data[i+18][j-1][k] + data[i+18][j][k-1];
                                        if (i+18 > 0 && k > 0)
                                          data[i+18][j][k] = (data[i+18][j][k] < data[i+18-1][j][k] + data[i+18][j][k-1])
                                                           ? data[i+18][j][k] : data[i+18-1][j][k] + data[i+18][j][k-1];
                                      }
                            }
                            {
                                if (i+19 > 1 && j > 1 && k > 1 && i+19 < DEPTH - 2 && j < HEIGHT - 2 && k < WIDTH - 2) {
                                        int candidate1 = data[i+19][j][k];                   
                                        int candidate2 = data[i+19][k][j] + data[k][j][i+19];   
                                        int candidate3 = data[i+19-1][j][k] + data[i+19][j-1][k]; 
                                        int candidate4 = data[i+19][j][k-1] + data[i+19-1][j-1][k-1]; 
                                        int new_val = candidate1;
                                        if (candidate2 < new_val) new_val = candidate2;
                                        if (candidate3 < new_val) new_val = candidate3;
                                        if (candidate4 < new_val) new_val = candidate4;
                                        data[i+19][j][k] = new_val;
                                        if (i+19 > 0 && j > 0)
                                          data[i+19][j][k] = (data[i+19][j][k] < data[i+19-1][j][k] + data[i+19][j-1][k])
                                                           ? data[i+19][j][k] : data[i+19-1][j][k] + data[i+19][j-1][k];
                                        if (j > 0 && k > 0)
                                          data[i+19][j][k] = (data[i+19][j][k] < data[i+19][j-1][k] + data[i+19][j][k-1])
                                                           ? data[i+19][j][k] : data[i+19][j-1][k] + data[i+19][j][k-1];
                                        if (i+19 > 0 && k > 0)
                                          data[i+19][j][k] = (data[i+19][j][k] < data[i+19-1][j][k] + data[i+19][j][k-1])
                                                           ? data[i+19][j][k] : data[i+19-1][j][k] + data[i+19][j][k-1];
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
