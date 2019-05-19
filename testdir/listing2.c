#include <stdio.h>
#include <stdlib.h>


/*********************************************
 * This file contains 1 basic implementation of
 * matrix multiply as the inner product with heap
 * space
 * *******************************************/
int main()
{
    int *a;
    int *b;
    int *c;

    posix_memalign((void **) &(a), 32, 100*10*sizeof(int));
    posix_memalign((void **) &(b), 32, 100*10*sizeof(int));
    posix_memalign((void **) &(c), 32, 100*100*sizeof(int));

    // Initialize matrices a and b;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 10; ++j) {
            *(a + i * 10 + j) = i * 10 + j;
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) {
            *(b + i * 100 + j) = i * 100 + j;
        }
    }


    // do matrix multiplication
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            for (int k = 0; k < 10; ++k) {
                *(c + i * 100 + j) += *(a + i * 10 + k) * *(b + k * 100 + j);
            }
        }
    }
}
