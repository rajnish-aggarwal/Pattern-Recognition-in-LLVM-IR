#include <stdio.h>


/*********************************************
 * This file contains 1 basic implementation of
 * matrix multiply as the inner product.
 * *******************************************/
int main()
{
    int a[100][10];
    int b[10][100];
    int c[100][100] = {0};

    // Initialize matrices a and b;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 10; ++j) {
            a[i][j] = i * 10 + j;
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) {
            b[i][j] = i * 10 + j;
        }
    }


    // do matrix multiplication
    for (int i = 0; i < 100; ++i) {
        for (int k = 0; k < 10; ++k) {
            for (int j = 0; j < 100; ++j) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}
