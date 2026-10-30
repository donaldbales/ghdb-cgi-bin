#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ghdb-cgi-bin.h"

/*
	gcc add.c -o add.cgi && chmod 755 add.cgi && sudo cp add.cgi /usr/lib/cgi-bin
*/

int main(int argc, char const *argv[])
{
	char *fb;
	char *fn = "/var/www/html/ghdbform.html";
	FILE *fp = NULL;
	char *p = NULL;;
	size_t br;
	struct stat s;
	int l;
	int i;
	int r;

	r = stat(fn, &s);
	if (r == 0) 
	{
		fb = malloc(s.st_size);
		if (fb == NULL) 
		{
			printf("Failed to allocate %ld bytes of memory for fb.\n", s.st_size);
			exit(EXIT_FAILURE);
		}
		//printf("s.st_size=%lu\n", s.st_size);
	}
	else
	{
		return 1;
	}

	fp = fopen(fn, "r");
	if (fp)
	{
		br = fread(fb, 1, s.st_size, fp);
		//printf("br=%lu\n", br);
		if (br != s.st_size)
		{
			printf("Didn't read the whole file.\n");
			exit(EXIT_FAILURE);
		}
	}
	fclose(fp);

	//puts(fb);
	for (i=0; i<NUM_FIELDS; i++)
	{
		p = strrep(fb, fields[i].cb, "");
		free(fb);
		fb = p;
	}
	p = strrep(fb, "{submit}", "Insert");
	free(fb);
	fb = p;
	p = strrep(fb, "action.cgi", "insert.cgi");
	free(fb);
	fb = p;

	printf("Content-Type:text/html;charset=iso-8859-1\n\n");

	puts(fb);

	free(fb);

	return 0;
}