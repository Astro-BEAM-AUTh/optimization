# Disable make's built-in variable and rules.
MAKEFLAGS += -rR
# Do not pollute shell with printing the CWD.
MAKEFLAGS += --no-print-directory

# Toolchain
CC = clang
HYPERFINE = hyperfine

# Toolchain flags.
CFLAGS = -std=c11 \
	-Wall -Wextra \
	-pipe

HFLAGS = -N \
	-r 256

# Recursive wildcard (unused for now).
# See: https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES = $(call rwildcard,.,*.c)

# Default rule.
all: naive with-native

# Build naive impls for opt levels 0..3.
naive:
	$(CC) $(CFLAGS) -O0 -o naive-opt0 naive.c
	$(CC) $(CFLAGS) -O1 -o naive-opt1 naive.c
	$(CC) $(CFLAGS) -O2 -o naive-opt2 naive.c
	$(CC) $(CFLAGS) -O3 -o naive-opt3 naive.c

# Build arch-native binaries of the naive impl (super-duper arch-dependant).
with-native:
	$(CC) $(CFLAGS) -O0 -march=native -o naive-native-opt0 naive.c
	$(CC) $(CFLAGS) -O1 -march=native -o naive-native-opt1 naive.c
	$(CC) $(CFLAGS) -O2 -march=native -o naive-native-opt2 naive.c
	$(CC) $(CFLAGS) -O3 -march=native -o naive-native-opt3 naive.c

# Generate assembler for all targets (for debugging purposes).
dbg-asm:
	$(CC) $(CFLAGS) -S -O0 -o naive-opt0.dbg.S naive.c
	$(CC) $(CFLAGS) -S -O1 -o naive-opt1.dbg.S naive.c
	$(CC) $(CFLAGS) -S -O2 -o naive-opt2.dbg.S naive.c
	$(CC) $(CFLAGS) -S -O3 -o naive-opt3.dbg.S naive.c
	$(CC) $(CFLAGS) -S -O0 -march=native -o naive-native-opt0.dbg.S naive.c
	$(CC) $(CFLAGS) -S -O1 -march=native -o naive-native-opt1.dbg.S naive.c
	$(CC) $(CFLAGS) -S -O2 -march=native -o naive-native-opt2.dbg.S naive.c
	$(CC) $(CFLAGS) -S -O3 -march=native -o naive-native-opt3.dbg.S naive.c


# Benchmarking rules
bench-naive:
	$(HYPERFINE) $(HFLAGS) './naive-opt0' './naive-opt1' './naive-opt2' './naive-opt3'

bench-with-native:
	$(HYPERFINE) $(HFLAGS) './naive-native-opt0' './naive-native-opt1' './naive-native-opt2' './naive-native-opt3'

clean:
	rm -f naive-*
	rm -f mag.txt

.PHONY: all clean
