#include <stdio.h>
#include <stdbool.h>

int main(void) {
    char c;
    double number;

    while (true) {
        c = getc(stdin);
        
        if (c == EOF) {
            break;
        }

        if (c == '-') { // if it's a negative number
            ungetc(c, stdin);  // push back the '-' to read the full number
            if (fscanf(stdin, "%lf", &number) == 1) {
                printf("Warning: Negative number (%lf) detected!\n", number);
                break;
            }
        } else if (c >= '0' && c <= '9') {
            ungetc(c, stdin);  // push back the digit to read the full number
            if (fscanf(stdin, "%lf", &number) == 1) {
                printf("Read number: %lf\n", number);
            }
        }
    }

    return 0;
}

