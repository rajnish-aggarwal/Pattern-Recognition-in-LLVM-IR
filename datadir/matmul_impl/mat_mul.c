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
    float *a;
    float *b;
    float *c;

    int size_x = 8196;
    int size_y = 128;
    int size_z = 256;

    int val1 = posix_memalign((void **) &(a), 32, size_x*size_y*sizeof(float));
    int val2 = posix_memalign((void **) &(b), 32, size_y*size_z*sizeof(float));
    int val3 = posix_memalign((void **) &(c), 32, size_x*size_z*sizeof(float));

    if (val1 != 0 || val2 != 0 || val3 != 0)
        printf("Memory allocation error\n");

    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            *(a + i * size_y + j) = i * 0.001 + j * 2.0;
        }
    }

    for (int i = 0; i < size_y; ++i) {
        for (int j = 0; j < size_z; ++j) {
            *(b + i * size_z + j) = i * 0.001 + j * 2.0;
        }
    }

    memset(c, 0, size_x*size_z*sizeof(float));

	unsigned long long st = rdtsc();
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_z; ++j) {
            for (int k = 0; k < size_y; ++k) {
                *(c + i * size_z + j) += *(a + i * size_y + k) * *(b + k * size_z + j);
            }
        }
    }

	printf("Total time: %llu\n\n\n\n", rdtsc() - st);

    FILE *f = fopen("clang_result.txt", "w");
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_z; ++j) {
            fprintf(f, "%lf ", *(c + i * size_z + j));
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return (0);
}
