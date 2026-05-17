#include "var_type_def.h"

void kernels_launch(const complex_float *input, const float *window0, const float *window1, const float *window2, 
                    const float *window3, complex_float *output, const size_t input_size, 
                    const size_t output_size, const size_t window_size);