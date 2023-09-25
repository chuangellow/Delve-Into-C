#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>

void no_file_name_quit() {
	fprintf(stderr, "Usage: ./calculate_mean <file_name>\n");
	exit(1);
}

void malloc_fail_quit() {
	fprintf(stderr, "Malloc failed\n");
	exit(1);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		no_file_name_quit();
		exit(1);
	}
	char *file_name = argv[1];
	FILE *file = fopen(file_name, "r");
	assert(file != NULL);
	int res = 1000;
	double *buffer = (double *) malloc(res * sizeof(double));
	if (buffer == NULL) {
		malloc_fail_quit();
	}
	int i = 0;
	double d, mean = 0;
	while (fscanf(file, "%lf", &d) == 1) {
		if (i == res) {
			res *= 2;
			buffer = (double *) realloc(buffer, res * sizeof(double));
			if (buffer == NULL) {
				malloc_fail_quit();
			}
		}
		buffer[i] = d;
		i++;
		mean = (i == 1) ? d: mean + (d - mean) / i;
	}
	printf("Mean: %lf\n", mean);
	free(buffer);
	return 0;
}
