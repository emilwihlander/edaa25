#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "error.h"
#include "poly.h"

struct poly_t
{
	int size;
	int *n;
};

poly_t *new_poly_from_string(const char *str)
{
	int *n = NULL;
	int size = 0;
	int x = 0;
	int sign = 1;
	for (int i = 0; 1; i++)
	{
		char c = str[i];
		if (isdigit(c))
		{
			x = 10 * x + (c - '0');
		}
		else if (c != '^' && c != ' ')
		{

			n = realloc(n, sizeof(int) * (size + 1));
			x = sign * ((!x) ? 1 : x);
			*(n + size++) = x;
			x = 0;

			sign = (c == 'x') ? 1 : -(c - 44);
		}

		if (!c)
		{
			n = realloc(n, sizeof(int) * (size + 1));
			*(n + size++) = 0;
			break;
		}
	}
	struct poly_t *poly = malloc(sizeof(struct poly_t));
	poly->n = n;
	poly->size = size;
	return poly;
}

void free_poly(poly_t *poly)
{
	free(poly->n);
	free(poly);
}

poly_t *mul(poly_t *a, poly_t *b)
{
	int *n = NULL;
	int size = 0;
	for (int i = 0; i < a->size; i += 2)
	{
		int j = 0;
		while (j < b->size)
		{
			int f = a->n[i] * b->n[j];
			int e = a->n[i + 1] + b->n[j + 1];

			for (int k = 0; k < size; k += 2)
			{
				if ((*(n + k + 1)) == e)
				{
					(*(n + k)) += f;
					goto END;
				}
			}
			n = realloc(n, sizeof(int) * (size + 2));
			*(n + size++) = f;
			*(n + size++) = e;
		END:
			j += 2;
		}
	}
	struct poly_t *poly = malloc(sizeof(struct poly_t));
	poly->n = n;
	poly->size = size;
	return poly;
}

void print_poly(poly_t *poly)
{
	int i = 0;
	while (i < poly->size - 2) {
		char *s = "";
		if (i != 0) {
			s = (poly->n[i] >= 0) ? " + " : " - ";
		}
		char d[9] = "";
		if (poly->n[i] != 1 && poly->n[i] != -1) snprintf(d,9,"%d", abs(poly->n[i]));

		if (i < poly->size - 4)
			printf("%s%sx^%d", s, d, poly->n[i+1]);
		else
			printf("%s%sx + ", s, d);
		i += 2;
	}

	printf("%d\n", poly->n[i]);
}