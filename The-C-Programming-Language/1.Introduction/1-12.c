#include <stdio.h>

#define IN 1
#define OUT 0

int main(void) {
	int c;
	int flag = 1;

	while ((c = getchar()) != EOF) {
		if (c == ' ' || c == '\t' || c == '\n') {
			flag = OUT;
		}
		else if (flag == OUT) {
			flag = IN;
			putchar('\n');
			putchar(c);
		}
		else {
			putchar(c);
		}
	}
	return 0;
}
