#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * html_encode(char *string)
{
	int string_length = strlen(string);
	char c;
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
			case '<':
				result_length += 3;
				break;
			case '>':
				result_length += 3;
				break;
			case '&':
				result_length += 4;
				break;
			case '"':
				result_length += 5;
				break;
			case '\'':
				result_length += 5;
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
			case '<':
				strcat(p, "&lt;");
				p += 4;
				break;
			case '>':
				strcat(p, "&gt;");
				p += 4;
				break;
			case '&':
				strcat(p, "&amp;");
				p += 5;
				break;
			case '"':
				strcat(p, "&quot;");
				p += 6;
				break;
			case '\'':
				strcat(p, "&apos;");
				p += 6;
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
	printf("%s\n", html_encode("Less than:<, greater than:>, ampersand:&, double quote:\", apostrophy:', done."));
	printf("%s\n", html_encode(""));

	return 0;
}