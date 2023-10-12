#include <stdio.h>

int main(void) {
	int c;
	int flag = 0;
	while ((c = getchar()) != EOF) {
		if (c == ' ' && !flag) {
			flag = 1;
			putchar(c);
		} 
		else if (c != ' '){
			flag = 0;
			putchar(c);
		}
	}
	return 0;
}
