#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * strrep(char *string, const char *search, const char *replace) {
	int length = 0;
	int offset = 0;
	int replace_length = strlen(replace);
	int search_length = strlen(search);
	int string_length = strlen(string);
	char *p;
	int occurs = 0;
	for (p = string; (p = strstr(p, search)) != NULL; p += search_length, occurs++);
	int result_length = string_length - (search_length * occurs) + (replace_length * occurs) + 1;
	char *result = malloc(result_length);
	char *buffer = malloc(string_length);
	char *b = buffer;

	strcpy(buffer, string);
	while ((p = strstr(buffer, search)) != NULL) {
		length = p - buffer;
		strncpy(result + offset, buffer, length);
		offset += length;
		strcpy(result + offset, replace);
		offset += replace_length;
		strcpy(buffer, p + search_length);
	}
	free(b);
	return result;
}

int main() {
	char *str = "Hello world, welcome to the C world!";
	printf("%s\n", strrep(str, "world", "universe"));
	return 0;
}
