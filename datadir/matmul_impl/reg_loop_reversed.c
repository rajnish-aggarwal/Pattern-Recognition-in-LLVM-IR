#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

int main()
{
    int size_x = 8196;
    int size_y = 128;
    int size_z = 256;

    float *a[size_x];
    float *b[size_y];
    float *c[size_x];

    for (int i = 0; i < size_x; ++i) {
        a[i] = (int *) malloc (size_y * sizeof(float));
    }

    for (int i = 0; i < size_y; ++i) {
        b[i] = (int *) malloc (size_z * sizeof(float));
    }

    for (int i = 0; i < size_x; ++i) {
        c[i] = (int *) malloc (size_z * sizeof(float));
    }


    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            a[i][j] = i * 0.001 + j * 2.0;
        }
    }

    for (int i = 0; i < size_y; ++i) {
        for (int j = 0; j < size_z; ++j) {
            b[i][j] = i * 0.001 + j * 2.0;
        }
    }

    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_z; ++j) {
            c[i][j] = 0.0;
        }
    }

	unsigned long long st = rdtsc();
    for (int i = 0; i < size_x; ++i) {
        for (int k = 0; k < size_y; ++k) {
            for (int j = 0; j < size_z; ++j) {
               c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

	printf("Total time: %llu\n\n\n\n", rdtsc() - st);

    FILE *f = fopen("clang_result.txt", "w");
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_z; ++j) {
            fprintf(f, "%lf ", c[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return (0);
}
