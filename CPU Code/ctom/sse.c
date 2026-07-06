#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

#define NSAMPLES 2500000

#include "samples.h"

FILE *fd;

int main()
{
    uintmax_t i;
    //static float samples[NSAMPLES][2];
    static float norm[NSAMPLES];
    __m128 x, y, r;

    fd = fopen("mag.txt", "w");
    if (!fd) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    
    //for (i = 0; i < NSAMPLES; i++) {
    //    samples[i][0] = 0.85f;
    //    samples[i][1] = 0.34f;
    //}

    for (i = 0; i < NSAMPLES - 4; i += 4) {
        x = _mm_set_ps(samples[i][0], samples[i+1][0], samples[i+2][0], samples[i+3][0]);
        y = _mm_set_ps(samples[i][1], samples[i+1][1], samples[i+2][1], samples[i+3][1]);
        x = _mm_mul_ps(x, x);
        y = _mm_mul_ps(y, y);
        r = _mm_add_ps(x, y);
        _mm_store_ps(norm + i, r);
    }

    for (i = 0; i < NSAMPLES; i++)
        fprintf(fd, "%0.6f\n", norm[i]);
    fclose(fd);

    return EXIT_SUCCESS;
}
