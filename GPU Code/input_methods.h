#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "var_type_def.h"

// Input methods
typedef enum {
    INPUT_FILE,
    INPUT_SDR,
    INPUT_SYNTHETIC
} input_mode_t;

size_t read_complex_dat_file(const char *filename, complex_float *buffer, size_t max_samples)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("Could not open input file: %s\n", filename);
        return 0;
    }

    size_t samples_read = fread(buffer, sizeof(complex_float), max_samples, fp);
    fclose(fp);
    return samples_read;
}

size_t read_complex_sdr(complex_float *buffer, size_t max_samples)
{
    printf("SDR input is not implemented yet.\n");
    return 0;
}