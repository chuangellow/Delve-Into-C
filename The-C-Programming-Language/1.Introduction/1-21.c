#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1000
#define DETAB 2

int get_line(char line[], int max_len) {
	int c, i;
	for (i = 0; i < max_len - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
		line[i] = c;
	}
	if (c == '\n') {
		line[i] = '\0';
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

void swap(char line[], int i, int j) {
	char tmp = line[i];
	line[i] = line[j];
	line[j] = tmp;
}

void reverse(char line[], int len) {
	if (len <= 1) return;
	int i, j;
	for (i = 0, j = len - 1; i < j; i++, j--) {
		swap(line, i, j);
	}
	return;
}

char* detab(char line[]) {
	char* p = malloc(sizeof(char) * MAXLINE);
	int i = 0, j = 0;
	while (line[i] != '\0') {
		if (line[i] == '\t') {
			int k;
			for (k = 0; k < DETAB; k++) {
				p[j++] = ' ';
			}
		}
		else {
			p[j++] = line[i];
		}
		i++;
	}
	p[j] = '\0';
	return p;
}

char* entab(char line[]) {
	char *p = (char*)malloc(sizeof(char) * MAXLINE);
	int i = 0, j = 0;
	while (line[i] != '\0') {
		if (line[i] == ' ') {
			while (line[i] == ' ') {
				i++;
			}
			p[j++] = '\t';
		}
		else {
			p[j++] = line[i++];
		}
	}
	p[j] = '\0';
	return p;
}

int main(void) {
	int len;
	char line[MAXLINE];
	while ((len = get_line(line, MAXLINE)) > 0) {
		if (len > 0) {
			printf("%s\n", line);
			char* entab_line = entab(line);
			char* detab_line = detab(entab_line);
			printf("%s\n", detab_line);
		}
	}
	return 0;
}
