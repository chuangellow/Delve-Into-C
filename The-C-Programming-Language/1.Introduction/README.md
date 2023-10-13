# Chapter 1. Introduction

## Exercise 1-1

> Run the ``hello, world'' program on your system. Experiment with leaving out parts of the program, to see what error messages you get.

## Exercise 1-2

> Experiment to find out what happens when prints's argument string contains \c, where c is some character not listed above.

Print the following line:

```
printf("Hello, world!\c");
```

The output is shown below:

```
Hello, world!c
```

During the compilation, the compiler will jump out the following warning:

```
warning: unknown escape sequence '\c' [-Wunknown-escape-sequence]
```

## Exercise 1-6

> Verify that the expression getchar() != EOF is 0 or 1.

We can print out the expression like:

```
int flag = getchar() != EOF;
printf("%d\n", flag);
```

After inputing any character, we will get a ```1```.

If we input a ```EOF``` character by typing ```ctrl+D```, we will get a ```0```.

## Exercise 1-7

> Write a program to print the value of ```EOF```

We can print the value of ```EOF``` by:

```
printf("value of EOF is: %d\n", EOF);
```

And the output is ```-1```. The value of ```EOF``` is pre-defined in ```<stdio.h>```.

Why the value of ```EOF``` is defined as ```-1```?

Because ```getchar``` will return any character value as an unsigned integer.

## Exercise 1-8

> Write a program to count blanks, tabs, and newlines.

```
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
```

## Exercise 1-9

> Write a program to copy its input to its output, replacing each string of one or more blanks by a single blank.

```
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
```

Here, we use a flag to indicate whether the blank is more than one or not.

## Exercise 1-10

> Write a program to copy its input to its output, replacing each tab by \t, each backspace by \b, and each backslash by \\. This makes tabs and backspaces visible in an unambiguous way.

```
#include <stdio.h>

int main(void) {
	int c;
	while ((c = getchar()) != EOF) {
		switch (c) {
			case '\t':
				putchar('\\');
				putchar('t');
				break;
			case '\b':
				putchar('\\');
				putchar('b');
				break;
			case '\\':
				putchar('\\');
				putchar('\\');
				break;
			default:
				putchar(c);
				break;

		}
	}
	return 0;
}
```

## Exercise 1-11

> How would you test the word count program? What kinds of input are most likely to uncover bugs if there are any?

Since the definition of a word is that any sequence of characters that doen't contain a blank, tab or newline.

The input like: ```hello_world``` may be counted as one word in that case.

Or another example like ```It's``` should be counted as two words, but in this definition, it will be counted as one word.

## Exercise 1-12

> Write a program that prints its input one word per line.

```
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
```

## Exercise 1-16

> Revise the main routine of the longest-line program so it will correctly print the length of arbitrary long input lines, and as much as possible of the text.

Since we cannot modify the implementation of the ```getline``` function, we can revise the main routine by:

1. After calling ```getline```, we can check whether the ```line[MAXLINE]``` is ```'\n'```.
2. If not, we can still call the ```getchar()``` and increment the length until it reaches the ```'\n'``` or ```EOF```.


## Exercise 1-17

> Write a program to print all input lines that are longer than 80 characters.

Suppose a line exceeds 80 characters, it will not encounter the newline character in the following line:

```
for (i = 0; i < max_len - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
	line[i] = c;
}
```
Therefore, we need to get the remaining characters in that line.

Moreover, we need to give an indicator to the function caller to let them know this line exceeds the length limit.

Therefore, we can return the length of the line to the caller as an indicator.

## Exercise 1-18

> Write a program to remove trailing blanks and tabs from each line of input, and to delete entirely blank lines.

Here we can implement the following function:

```
int remove_blank(char line[], int len) {
	int i;
	for (i = len - 2; i >= 0 && line[i] == ' '; i--) {
		line[i] = '\0';
	}
	return i+1;
}
```

However, the function is dependent on whether the argument ```len``` counts the last ```\0```.

It's not a good implementation. We can revise this into:

```
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
```

## Exercise 1-19

> Write a function reverse(s) that reverses the character string s. Use it to write a program that reverses its input a line at a time.

## Exercise 1-20

> Write a program detab that replaces tabs in the input with the proper number of blanks to space to the next tab stop. Assume a fixed set of tab stops, say every n columns. Should n be a variable or a symbolic parameter?


## Exercise 1-21

> Write a program entab that replaces strings of blanks by the minimum number of tabs and blanks to achieve the same spacing. Use the same tab stops as for detab. When either a tab or a single blank would suffice to reach a tab stop, which should be given preference?

## Exercise 1-22

> Write a program to ``fold'' long input lines into two or more shorter lines after the last non-blank character that occurs before the n-th column of input. Make sure your program does something intelligent with very long lines, and if there are no blanks or tabs before the specified column.

## 