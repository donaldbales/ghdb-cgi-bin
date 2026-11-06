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
	FILE *file_pointer = NULL;

	file_buffer = get_asset(file_name);

	//puts(file_buffer);
	for (i=0; i<NUM_FIELDS; i++)
	{
		pointer = strrep(file_buffer, fields[i].cb, "");
		free(file_buffer);
		file_buffer = pointer;
	}
	pointer = strrep(file_buffer, "{submit}", "Find");
	free(file_buffer);
	file_buffer = pointer;
	pointer = strrep(file_buffer, "action.cgi", "select.cgi");
	free(file_buffer);
	file_buffer = pointer;

	printf("Content-Type:text/html;charset=iso-8859-1\n\n");

	puts(file_buffer);

	free(file_buffer);

	return 0;
}