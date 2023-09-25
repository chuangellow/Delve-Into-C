#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

void quit() {
	fprintf(stderr, "memory exhausted\n");
	exit(1);
}

int main(void) {
	int max = 20;
	char *name = (char *)malloc(max);
	if (name == NULL) quit();

	printf("What is your name? ");

	// skip leading whitespace
	while (true) {
		int c = getchar();
		if (c == EOF) break;
		if (!isspace(c)) {
			ungetc(c, stdin);
			break;
		}
	}

	int i = 0;
	while (true) {
		int c = getchar();
		if (c == EOF || c == '\n') {
			name[i] = 0;
			break;
		}
		name[i] = c;
		// Reallocate the array if out of space
		if (i >= max) {
			max *= 2;
			name = (char *)realloc(name, max);
			if (name == NULL) quit();
		}
		i++;
	}
	printf("%s\n", name);
	return 0;
}
