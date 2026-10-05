import struct
import random

total_values = 50 * 4096 * 2
with open("input.dat", "wb") as f:
    for _ in range(total_values):
        val = random.randint(-1000, 1000)
        f.write(struct.pack("<h", val))

print("input.dat created!")
