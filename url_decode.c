#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
	https://developer.mozilla.org/en-US/docs/Glossary/Percent-encoding
	gcc url_decode.c -o url_decode
*/

char * url_decode(char *string)
{
	int string_length = strlen(string);
	char c;
	char c1;
	char c2;
	char x2i[3];
	char *p;
	char *result = NULL;
	int result_length = string_length;
	int i = 0;
	for (i=0; i<string_length; i++)
	{
		c = *(string + i);
		//printf("%c\n", c);
		switch (c)
		{
			case '%':
				if ((i + 2) < string_length)
				{
					c1 = *(string + i + 1);
					c2 = *(string + i + 2);
					if (isxdigit(c1) &&
						isxdigit(c2))
					{
						result_length +- 2;
					}
				}
				break;
			case '+':
				break;
		}
	}
	result = malloc(result_length + 1);
	p = result;
	for (i=0; i<string_length; i++)
	{
		c = *(string + i);
		//printf("%c\n", c);
		switch (c)
		{
			case '%':
				if ((i + 2) < string_length)
				{
					c1 = *(string + i + 1);
					c2 = *(string + i + 2);
					if (isxdigit(c1) &&
						isxdigit(c2))
					{
						x2i[0] = c1;
						x2i[1] = c2;
						x2i[2] = '\0';
						*p = (char) strtol(x2i, NULL, 16);
						p += 1;
						i += 2;
					}
				}
				break;
			case '+':
				*p = ' ';
				p += 1;
				break;
			default:
				*p = c;
				p += 1;
				break;
		}
	}

	return result;
}

int main(int argc, char const *argv[])
{
	printf("%s\n", url_decode("Colon%3A%3A%2C+forward+slash%3A%2F%2C+question_mark%3A%3F%2C+ampersand%3A%26%2C+apostrophy%3A%27%2C done."));
	printf("%s\n", url_decode(""));

	return 0;
}