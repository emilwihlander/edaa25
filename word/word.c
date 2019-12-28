#include <stdio.h>

int main(void)
{
	int c;
	int counter = 0;
	int pos = 0;
	int m_p = 0;
	int m_s = 0;
	char *str = NULL;

	while ((c = getchar()) != EOF)
	{
		str = realloc(str, counter + 1);
		*(str + counter++) = c;
		if (c < 'A')
		{
			if (counter - pos > m_s)
			{
				m_p = counter;
				m_s = counter - pos;
			}
			pos = counter + 1;
		}
	}
	printf("%d characters in longest word: %.*s\n", m_s, m_s, str + m_p);
	free(str);
}