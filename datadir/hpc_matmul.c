#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <immintrin.h>

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

void kernel(float    *a,
            float    *b,
            float    *c,
            int      size_x,
            int      size_y,
            int      size_z)
{

    float *temp_a = a;
    float *temp_b = b;
    float *temp_c1 = c;
    float *temp_c2 = c + size_z;
    float *temp_c3 = c + 2*size_z;
    float *temp_c4 = c + 3*size_z;
    float *temp_c5 = c + 4*size_z;
    float *temp_c6 = c + 5*size_z;

    __m256 in_a1, in_a2, in_a3, in_a4, in_a5, in_a6;
    __m256 in_b1, in_b2;
    __m256 out1, out2, out3, out4, out5, out6,
           out7, out8, out9, out10, out11, out12;

    out1 = _mm256_setzero_ps();
    out2 = _mm256_setzero_ps();
    out3 = _mm256_setzero_ps();
    out4 = _mm256_setzero_ps();
    out5 = _mm256_setzero_ps();
    out6 = _mm256_setzero_ps();
    out7 = _mm256_setzero_ps();
    out8 = _mm256_setzero_ps();
    out9 = _mm256_setzero_ps();
    out10 = _mm256_setzero_ps();
    out11 = _mm256_setzero_ps();
    out12 = _mm256_setzero_ps();


	unsigned long long st = rdtsc();
    for (int i = 0; i < size_x; i += 6) {
        for (int j = 0; j < size_z; j += 16) {
            temp_b = b + j;
            temp_a = a + i * size_y;
            for (int k = 0; k < size_y; ++k) {
                // Using a 6x16 kernel
                in_a1 = _mm256_broadcast_ss(temp_a);
                in_a2 = _mm256_broadcast_ss(temp_a + size_y);
                in_a3 = _mm256_broadcast_ss(temp_a + (size_y << 1));
                in_a4 = _mm256_broadcast_ss(temp_a + (size_y << 1) + size_y);
                in_a5 = _mm256_broadcast_ss(temp_a + (size_y << 2));
                in_a6 = _mm256_broadcast_ss(temp_a + (size_y << 2) + size_y);

                in_b1 = _mm256_load_ps(temp_b);
                in_b2 = _mm256_load_ps(temp_b + 8);

                out1 = _mm256_fmadd_ps(in_a1, in_b1, out1);
                out2 = _mm256_fmadd_ps(in_a1, in_b2, out2);

                out3 = _mm256_fmadd_ps(in_a2, in_b1, out3);
                out4 = _mm256_fmadd_ps(in_a2, in_b2, out4);

                out5 = _mm256_fmadd_ps(in_a3, in_b1, out5);
                out6 = _mm256_fmadd_ps(in_a3, in_b2, out6);

                out7 = _mm256_fmadd_ps(in_a4, in_b1, out7);
                out8 = _mm256_fmadd_ps(in_a4, in_b2, out8);

                out9 = _mm256_fmadd_ps(in_a5, in_b1, out9);
                out10 = _mm256_fmadd_ps(in_a5, in_b2, out10);

                out11 = _mm256_fmadd_ps(in_a6, in_b1, out11);
                out12 = _mm256_fmadd_ps(in_a6, in_b2, out12);

                ++temp_a;
                temp_b += size_z;
            }

            _mm256_store_ps(temp_c1, out1);
            _mm256_store_ps(temp_c1 + 8, out2);
            _mm256_store_ps(temp_c2, out3);
            _mm256_store_ps(temp_c2 + 8, out4);
            _mm256_store_ps(temp_c3, out5);
            _mm256_store_ps(temp_c3 + 8, out6);
            _mm256_store_ps(temp_c4, out7);
            _mm256_store_ps(temp_c4 + 8, out8);
            _mm256_store_ps(temp_c5, out9);
            _mm256_store_ps(temp_c5 + 8, out10);
            _mm256_store_ps(temp_c6, out11);
            _mm256_store_ps(temp_c6 + 8, out12);

            temp_c1 += 16;
            temp_c2 += 16;
            temp_c3 += 16;
            temp_c4 += 16;
            temp_c5 += 16;
            temp_c6 += 16;

            out1 = _mm256_setzero_ps();
            out2 = _mm256_setzero_ps();
            out3 = _mm256_setzero_ps();
            out4 = _mm256_setzero_ps();
            out5 = _mm256_setzero_ps();
            out6 = _mm256_setzero_ps();
            out7 = _mm256_setzero_ps();
            out8 = _mm256_setzero_ps();
            out9 = _mm256_setzero_ps();
            out10 = _mm256_setzero_ps();
            out11 = _mm256_setzero_ps();
            out12 = _mm256_setzero_ps();
        }
        temp_c1 += 5*size_z;
        temp_c2 += 5*size_z;
        temp_c3 += 5*size_z;
        temp_c4 += 5*size_z;
        temp_c5 += 5*size_z;
        temp_c6 += 5*size_z;
    }

	printf("Total time: %llu\n\n\n\n", rdtsc() - st);

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

    // Insert a function call to the HPC kernel
    kernel(a, b, c, size_x, size_y, size_z);

    FILE *f = fopen("hand_opt_result.txt", "w");

    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_z; ++j) {
            fprintf(f, "%lf ", *(c + i * size_z + j));
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return (0);
}
