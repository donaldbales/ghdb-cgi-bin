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
gcc find.c -I/usr/include/mysql -lmysqlclient -o find.cgi && chmod 755 find.cgi && sudo cp find.cgi /usr/lib/cgi-bin

*/

int main(int argc, char const *argv[])
{
	int i = 0;

	char *file_buffer= NULL;
	char *file_name = "/var/www/html/ghdbform.html";
	char *pointer = NULL;
	char *temp_buffer = NULL;

	/*
		Get the HTML template
	*/
	file_buffer = get_asset(file_name);


	temp_buffer = malloc(strlen(file_buffer) + 1);
	if (temp_buffer == NULL) 
	{
		printf("Failed to allocate %ld bytes of memory for temp_buffer.\n", strlen(file_buffer) + 1);
		exit(EXIT_FAILURE);
	}
	strcpy(temp_buffer, file_buffer);

	//puts(temp_buffer);
	for (i=0; i<NUM_FIELDS; i++)
	{
		pointer = strrep(temp_buffer, fields[i].cb, "");
		free(temp_buffer);
		temp_buffer = pointer;
	}
	pointer = strrep(temp_buffer, "{submit}", "Find");
	free(temp_buffer);
	temp_buffer = pointer;
	pointer = strrep(temp_buffer, "action.cgi", "select.cgi");
	free(temp_buffer);
	temp_buffer = pointer;

	printf("Content-Type:text/html;charset=iso-8859-1\n\n");
	puts(temp_buffer);
	free(temp_buffer);


	return 0;
}