#include <stdio.h>

#define MAXLINE 1000

int get_line(char line[], int max_len) {
	int c, i;
	for (i = 0; i < max_len - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
		line[i] = c;
	}
	if (c == '\n') {
		line[i] = '\0';
		i++;
	} 
	else if (c == EOF) return -1;
	else {
		while ((c = getchar()) != EOF && c != '\n') {
			i++;
		}
	}
	return i;
}

int remove_blank(char line[], int len) {
	int last_pos = len - 1;
	while (last_pos >= 0 && line[last_pos] == '\0') {
		last_pos--;
	}
	int i;
	for (i = last_pos; i >= 0 && line[i] == ' '; i--) {
		line[i] = '\0';
	}
	return i+1;
}

int main(void) {
	int len;
	char line[MAXLINE];
	while ((len = get_line(line, MAXLINE)) > 0) {
		len = remove_blank(line, len);
		if (len > 0) {
			printf("%s\n", line);
		}
	}
	return 0;
}
