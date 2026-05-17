#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "var_type_def.h"
#include "GPU_Kernel_Calls.cuh"
#include "input_methods.h"

#define VEC_LENGTH 1024
#define NUM_FRAMES 100000


int main(int argc, char **argv) 
{
    int input_vectors = NUM_FRAMES + 3;
    size_t input_size = input_vectors * VEC_LENGTH * sizeof(complex_float);
    size_t output_size = NUM_FRAMES * VEC_LENGTH * sizeof(float);
    size_t window_size = VEC_LENGTH * sizeof(float);

    complex_float *input = malloc(input_size);
    float *output = malloc(output_size);
    float *window0 = malloc(window_size);
    float *window1 = malloc(window_size);
    float *window2 = malloc(window_size);
    float *window3 = malloc(window_size);

    if (!input || !output || !window0 || !window1 || !window2 || !window3) 
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    size_t total_input_samples = (size_t)input_vectors * VEC_LENGTH;
    size_t samples_read = 0;

    // Input
    input_mode_t mode = INPUT_SYNTHETIC;
    if (argc >= 2)
    {
        if (strcmp(argv[1], "file") == 0)
            mode = INPUT_FILE;
        else if (strcmp(argv[1], "sdr") == 0)
            mode = INPUT_SDR;
        else if (strcmp(argv[1], "synthetic") == 0)
            mode = INPUT_SYNTHETIC;
    }

    if (mode == INPUT_FILE)
    {
        if (argc < 3)
        {
            printf("Usage: %s file input.dat\n", argv[0]);
            return 1;
        }
        samples_read = read_complex_dat(argv[2], input, total_input_samples);

        if (samples_read < total_input_samples)
        {
            for (size_t i = samples_read; i < total_input_samples; i++)
            {
                input[i].re = 0.0f;
                input[i].im = 0.0f;
            }
        }
    } else if (mode == INPUT_SDR)
    {
        samples_read = read_complex_sdr(input, total_input_samples);

        if (samples_read < total_input_samples)
        {
            for (size_t i = samples_read; i < total_input_samples; i++)
            {
                input[i].re = 0.0f;
                input[i].im = 0.0f;
            }
        }
    } else
    {
        for (size_t i = 0; i < total_input_samples; i++)
        {
            input[i].re = 0.0f;
            input[i].im = 0.0f;
        }
    }

    for (int i = 0; i < VEC_LENGTH; i++) 
    {
        window0[i] = 0.25f;
        window1[i] = 0.25f;
        window2[i] = 0.25f;
        window3[i] = 0.25f;
    }
    kernels_launch(input, window0, window1, window2, window3, output, input_size, output_size,
                   window_size, NUM_FRAMES);

    

    
    // Memory cleanup
    free(input);
    free(output);
    free(window0);
    free(window1);
    free(window2);
    free(window3);
    return 0;
}