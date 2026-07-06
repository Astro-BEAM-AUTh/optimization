#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define abs(x) \
	((x) < 0 ? -(x) : (x))

#define pow2(x) \
	(x) * (x)

#define NSAMPLES 2500000

/*
 * XXX - for testing purposes.
 * The file is 60MB so...
 */
#include "samples.h"

/* Compile-time allocated buffer. */
static float mag[NSAMPLES] = { 0 };

/* 
 * We _need_ to write the `mag` array to a file or
 * else the compiler optimizes-out the loop :(
 */
FILE *fd;

int main()
{
	size_t i;
	float x, y;
	
	fd = fopen("mag.txt", "w");
	if (fd == NULL) {
		perror("fopen");
		return EXIT_FAILURE;
	}

	for (i = 0; i < NSAMPLES; i++) {
		x = samples[i][0];
		y = samples[i][1];
		mag[i] = pow2(abs(x)) + pow2(abs(y));
		fprintf(fd, "%0.6f\n", mag[i]);
	}

	fclose(fd);
	return EXIT_SUCCESS;
}
