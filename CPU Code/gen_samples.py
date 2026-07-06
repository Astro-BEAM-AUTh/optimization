import random as rand

MAX_ITER = 2_500_000

print('#pragma once')
print(f'const float samples[{MAX_ITER}][2] = {{')
for i in range(MAX_ITER):
    x, y = rand.uniform(0, 1), rand.uniform(0, 1)
    vec = '\t{{ {0:0.6f}, {1:0.6f} }},'
    print(vec.format(x, y))
print('};')