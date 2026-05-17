# GPU SDR documentation

## Main pipeline structure

## CPU part
.dat file/synthetic data/SDR input => complex_float input buffer =>

### => data tranfser from CPU to GPU =>

## GPU part

=> 4 vectors of 1024 complex samples => windowing => sum per sample index => complex float output buffer => FFT => magnitude^2 => integration => log10 =>

### => data tranfser from GPU to CPU =>

## CPU part

=> output file

### Reasoning behind the splitting

The CPU is responsible for data acquisition, control flow and final output handling, while the GPU is used for the computationally intensive DSP stages.