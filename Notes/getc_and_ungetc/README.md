# Note on the ```ungetc()``` and ```getc()```

## ```getc()```

根據下面的 man page 可以看到：
[getc(3) - Linux man page](https://linux.die.net/man/3/getc)

```getc()``` 會從指定的 stream 中讀入一個 character ，並且返回該讀入結果並做 type casting 為 unsigned character，或者是 end of file。

和 ```fgetc()``` 的差別在於，getc() 可以被實作為 macro。

## ```ungetc()```

```ungetc()``` 就相反，將一個 character 放回指定的 stream 中，其 prototype 為：

```
#include <stdio.h>
int ungetc(int c, FILE *stream);
```

下面為一例，當遇到負數時，需要跳 warning 出來，這時，我們就能使用 ```ungetc()``` 將負號放回```stdin``` 中。

```
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
```