#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mysql.h"
#include "ghdb-cgi-bin.h"

/*
	gcc add.c -I/usr/include/mysql -lmysqlclient -o add.cgi && chmod 755 add.cgi && sudo cp add.cgi /usr/lib/cgi-bin
*/

int main(int argc, char const *argv[])
{
	int i = 0;

	struct stat file_status;

	char *file_buffer= NULL;
	char *file_name = "/var/www/html/ghdbform.html";
	char *pointer = NULL;
	FILE *file_pointer = NULL;
	size_t bytes_read = 0;

	i = stat(file_name, &file_status);
	if (i == 0) 
	{
		file_buffer = malloc(file_status.st_size);
		if (file_buffer == NULL) 
		{
			printf("Failed to allocate %ld bytes of memory for file_buffer.\n", file_status.st_size);
			exit(EXIT_FAILURE);
		}
		//printf("file_status.st_size=%lu\n", file_status.st_size);
	}
	else
	{
		return 1;
	}

	file_pointer = fopen(file_name, "r");
	if (file_pointer)
	{
		bytes_read = fread(file_buffer, 1, file_status.st_size, file_pointer);
		//printf("bytes_read=%lu\n", bytes_read);
		if (bytes_read != file_status.st_size)
		{
			printf("Didn't read the whole file.\n");
			exit(EXIT_FAILURE);
		}
	}
	fclose(file_pointer);

	//puts(file_buffer);
	for (i=0; i<NUM_FIELDS; i++)
	{
		pointer = strrep(file_buffer, fields[i].cb, "");
		free(file_buffer);
		file_buffer = pointer;
	}
	pointer = strrep(file_buffer, "{submit}", "Insert");
	free(file_buffer);
	file_buffer = pointer;
	pointer = strrep(file_buffer, "action.cgi", "insert.cgi");
	free(file_buffer);
	file_buffer = pointer;

	printf("Content-Type:text/html;charset=iso-8859-1\n\n");

	puts(file_buffer);

	free(file_buffer);

	return 0;
}