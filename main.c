#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_LENGTH 1024
#define NUM_FRAMES 100000

// Custom type for complex input
typedef struct {
    float re;
    float im;
} complex_float;

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


void add_blocks(const complex_float *input, const float *window0, const float *window1, 
                             const float *window2, const float *window3, complex_float *output, int num_frames) 
{
    for (int frame = 0; frame < num_frames; frame++) 
    {
        int out_base = frame * VEC_LENGTH;
        int b0_base = (frame + 0) * VEC_LENGTH;
        int b1_base = (frame + 1) * VEC_LENGTH;
        int b2_base = (frame + 2) * VEC_LENGTH;
        int b3_base = (frame + 3) * VEC_LENGTH;

        for (int i = 0; i < VEC_LENGTH; i++) 
        {
            output[out_base + i].re =
                input[b0_base + i].re * window0[i] +
                input[b1_base + i].re * window1[i] +
                input[b2_base + i].re * window2[i] +
                input[b3_base + i].re * window3[i];

            output[out_base + i].im =
                input[b0_base + i].im * window0[i] +
                input[b1_base + i].im * window1[i] +
                input[b2_base + i].im * window2[i] +
                input[b3_base + i].im * window3[i];
        }
    }
}

int main(int argc, char **argv) 
{
    int input_vectors = NUM_FRAMES + 3;
    size_t input_size = input_vectors * VEC_LENGTH * sizeof(complex_float);
    size_t output_size = NUM_FRAMES * VEC_LENGTH * sizeof(complex_float);
    size_t window_size = VEC_LENGTH * sizeof(float);

    complex_float *input = malloc(input_size);
    complex_float *output = malloc(output_size);
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
            for (size_t i = 0; i < total_input_samples; i++)
            {
                input[i].re = (float)i * 0.000f;
                input[i].im = (float)i * 0.000f;
            }
        }
    } else if (mode == INPUT_SDR)
    {
        samples_read = read_complex_sdr(input, total_input_samples);

        if (samples_read < total_input_samples)
        {
            for (size_t i = 0; i < total_input_samples; i++)
            {
                input[i].re = (float)i * 0.000f;
                input[i].im = (float)i * 0.000f;
            }
        }
    } else
    {
        for (size_t i = 0; i < total_input_samples; i++)
        {
            input[i].re = (float)i * 0.001f;
            input[i].im = (float)i * 0.002f;
        }
    }

    for (int i = 0; i < VEC_LENGTH; i++) 
    {
        window0[i] = 0.25f;
        window1[i] = 0.25f;
        window2[i] = 0.25f;
        window3[i] = 0.25f;
    }
    add_blocks(input, window0, window1, window2, window3, output, NUM_FRAMES);

    //for (int i = 0; i < 5; i++) 
      //  printf("output[%d] = %f + %fi\n", i, output[i].re, output[i].im);


    
    // Memory cleanup
    free(input);
    free(output);
    free(window0);
    free(window1);
    free(window2);
    free(window3);
    return 0;
}