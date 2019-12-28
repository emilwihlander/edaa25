#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#define N (10)

struct stack
{
	int top;
	int *array;
};

struct stack *create_stack()
{
	struct stack *s = (struct stack *)malloc(sizeof(struct stack));
	s->top = -1;
	s->array = (int *)malloc(N * sizeof(int));
	return s;
}

void free_stack(struct stack *s)
{
	free(s->array);
	free(s);
}

int is_full(struct stack *s)
{
	return s->top == N - 1;
}

int is_empty(struct stack *s)
{
	return s->top == -1;
}

void push(struct stack *s, int item)
{
	s->array[++s->top] = item;
}

int pop(struct stack *s)
{
	return s->array[s->top--];
}

int to_number(struct stack *s)
{
	int factor = 1;
	int result = 0;
	while (!is_empty(s))
	{
		result += pop(s) * factor;
		factor *= 10;
	}
	return result;
}

void reset_stack(struct stack *s)
{
	while (!is_empty(s))
	{
		pop(s);
	}
}

void print_line(int counter)
{
	printf("line %d: ", counter);
}

void print_error(int counter)
{
	print_line(counter);
	printf("error at ");
}

void print_result(int counter, int result)
{
	print_line(counter);
	printf("%d\n", result);
}

int main(void)
{
	struct stack *s = create_stack();
	struct stack *intarr = create_stack();
	int counter = 1;
	int failed = 0;
	int c;
	while ((c = getchar()) != EOF)
	{
		if (!isdigit(c) && !is_empty(intarr))
		{
			int number = to_number(intarr);
			if (is_full(s))
			{
				print_error(counter);
				printf("%d\n", number);
				failed = 1;
			}
			else
			{
				push(s, number);
			}
		}

		if (c == '\n')
		{
			if (!failed)
			{
				if (s->top != 0)
				{
					print_error(counter);
					printf("%s\n", "\\n");
					failed = 1;
				}
				else
				{
					print_result(counter, pop(s));
				}
			}

			counter++;
			failed = 0;
			reset_stack(s);
		}
		else if (!failed)
		{
			if (isdigit(c))
			{
				c -= '0';
				push(intarr, c);
			}
			else if ((c == '+' || c == '-' || c == '*' || c == '/') && s->top >= 1)
			{
				int a = pop(s);
				int b = pop(s);
				if (c == '+')
					push(s, b + a);
				else if (c == '-')
					push(s, b - a);
				else if (c == '*')
					push(s, b * a);
				else if (a != 0)
					push(s, b / a);
				else
				{
					print_error(counter);
					printf("%c\n", c);
					failed = 1;
				}
			}
			else if (c != ' ')
			{
				print_error(counter);
				printf("%c\n", c);
				failed = 1;
			}
		}
	}
	free_stack(s);
	free_stack(intarr);
	return 0;
}
