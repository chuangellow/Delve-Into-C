#include <stdio.h>

int main(void) {
	int blanks, tabs, newlines;
	blanks = tabs = newlines = 0;
	int c;

	while ((c = getchar()) != EOF) {
		if (c == ' ') {
			blanks++;
		} else if (c == '\t') {
			tabs++;
		} else if (c == '\n') {
			newlines++;
		}
	}
	printf("blanks: %d, tabs: %d, newlines: %d\n", blanks, tabs, newlines);
}
