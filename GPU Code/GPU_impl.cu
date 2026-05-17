#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "var_type_def.h"
#include "GPU_Kernel_Calls.cuh"

// CUDA Error Checking
static void cudaErrorCheck(cudaError_t err, const char* msg)
{
	if (err != cudaSuccess) {
		printf("Error: %s -> %s \n\n", msg, cudaGetErrorString(err));
	    exit(1);
	}
}




void kernels_launch(const complex_float* input, const float* window0, const float* window1, const float* window2, 
                    const float* window3, float* output, const size_t input_size, 
                    const size_t output_size, const size_t window_size, const int NUM_FRAMES)
{
    complex_float* d_input, * d_window_sum_output;
    cudaErrorCheck(cudaMalloc((void**)&d_input, input_size), "d_input malloc");
    cudaErrorCheck(cudaMalloc((void**)&d_window_sum_output, output_size), "d_window_sum_output malloc");


}
