#include <stdio.h>

int main(void) {
	const int max = 20;
	char name[max];

	printf("Enter your name: ");
	char fmt[10];
	sprintf(fmt, "%%%ds", max - 1); // create a format string: plain %s can overflow
	scanf(fmt, name); // read at most max - 1 characters into name
	printf("Hello, %s!\n", name);
	return 0;
}
