#include <stdio.h>

int get_line(char line[], int max_len) {
	int c, i;
	for (i = 0; i < max_len - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
		line[i] = c;
	}
	if (c == '\n') {
		line[i] = '\0';
		return i;
	}
	else if (c == EOF) return -1;
	else {
		while ((c = getchar()) != EOF && c != '\n') {
			i++;
		}
		return i;
	}
}

void print_line(char line[], int max_len) {
	for (int i = 0; i < max_len && line[i] != '\0'; i++) {
		putchar(line[i]);
	}
	putchar('\n');
	return;
}

int main(void) {
	int len = 0;
	int max_len = 81;
	char line[max_len];
	while ((len = get_line(line, max_len)) != -1) {
		printf("len: %d\n", len);
		if (len >= max_len) continue;
		print_line(line, max_len);
	}
	return 0;
}
