#include <stdio.h>

#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "mysql.h"
#include "ghdb-cgi-bin.h"

/*
gcc delete.c -o delete.cgi -I/usr/include/mysql -lmysqlclient && chmod 755 delete.cgi && sudo cp delete.cgi /usr/lib/cgi-bin

cat Apple.delete | env CONTENT_LENGTH=`stat -c %s Apple.delete` ./delete.cgi
*/

int main(int argc, char const *argv[])
{
	int i = 0;

	int key_values_length = 0;
	struct KEY_VALUE *key_values = NULL;

	char **values = NULL;
	int field_index = 0;
	int values_index = 0;
	int values_length = 0;

	MYSQL *mysql = NULL;
	MYSQL_STMT *stmt = NULL;

	MYSQL_BIND *bind_params = NULL;
	bool *bind_param_is_null = NULL;
	char *bind_param_buffer = NULL;
	long unsigned *bind_param_length = NULL;
	int bind_params_index = 0;

	uint64_t affected_rows = 0;

	/*
		Get a connection to MySQL
	*/
	mysql = get_mysql_connection();
	if (mysql == NULL)
	{
		dprintf(2, "%s: Failed to acquire a MySQL connection.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (debug > 0) dprintf(2, "Initialize a new DELETE prepared statement.\n");
	stmt = mysql_stmt_init(mysql);
	if (mysql_stmt_errno(stmt) != 0)
	{
		dprintf(2, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}
	if (debug > 0) dprintf(2, "New prepared statement: %p\n", stmt);

	if (debug > 0) dprintf(2, "Prepare the statement.\n");
	i = mysql_stmt_prepare(stmt, ghdb_delete, strlen(ghdb_delete));
	if (mysql_stmt_errno(stmt) != 0)
	{
		dprintf(2, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	/*
		Get the POST key value pairs
	*/
	key_values_length = 0;
	values_length = 0;
	key_values = get_post_data(&key_values_length);
	if (debug > 0) fprintf(stderr, "key_values_length=%d\n", key_values_length);
	if (key_values_length > 0)
	{
		values = (char **)malloc(sizeof(char *) * NUM_FIELDS);
		if (values == NULL)
		{
			fprintf(stderr, "Failed to allocate values memory.\n");
			exit(EXIT_FAILURE);
		}
		for (i = 0; i < NUM_FIELDS; i++)
		{
			*(values + i) = (char *)malloc(sizeof(char) * 2001);
			if (*(values + i) == NULL)
			{
				fprintf(stderr, "Failed to allocate value memory.\n");
				exit(EXIT_FAILURE);
			}
			strcpy(*(values + i), "");
		}
		for (i = 0; i < key_values_length; i++)
		{
			field_index = get_field_index((key_values + i)->key);
			if (debug > 0) fprintf(stderr, "field_index=%d\n", field_index);
			if (debug > 0) fprintf(stderr, "(key_values + i)->value=%s\n", (key_values + i)->value);
			if (debug > 0)
			{
				temp = url_decode((key_values + i)->value);
				fprintf(stderr, "url_decode((key_values + i)->value)=%s\n", temp);
				free(temp);
			}
			if (field_index > -1)
			{
				values_length++;
				temp = url_decode((key_values + i)->value);
				strcpy(*(values + field_index), temp);
				free(temp);
				if (debug > 0) fprintf(stderr, "*(values + %d)=\"%s\"\n", field_index, *(values + field_index));
			} 
		}
	}

	if (debug > 0) fprintf(stderr, "Build the bind_param variable.\n");
	bind_params = (struct MYSQL_BIND *)malloc(sizeof(struct MYSQL_BIND) * NUM_FIELDS);
	if (bind_params == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_params memory.\n");
		exit(EXIT_FAILURE);
	}
	if (debug > 0) fprintf(stderr, "Set the bind_param array to zeroes.\n");
	memset(bind_params, 0, sizeof(struct MYSQL_BIND) * NUM_FIELDS);
	if (debug > 0) fprintf(stderr, "bind_params=%p\n", bind_params);

	bind_params_index = -1;

	/*
		Now set the where clause
	*/
	if (debug > 0) fprintf(stderr, "Initialize the bind_params structure for %s.\n", fields[0].n);

	if (debug > 0) fprintf(stderr, "Allocate bind_param_buffer memory for %s.\n", *(values + 0));
	bind_param_buffer = malloc(strlen(*(values + 0)) + 1);
	if (bind_param_buffer == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_param_buffer memory.\n");
		exit(EXIT_FAILURE);
	}
	strcpy(bind_param_buffer, *(values + 0));

	if (debug > 0) fprintf(stderr, "Allocate bind_param_length memory for %ld.\n", strlen(*(values + 0)));
	bind_param_length = malloc(sizeof(unsigned long int));
	if (bind_param_length == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_param_length memory.\n");
		exit(EXIT_FAILURE);
	}
	*(bind_param_length) = strlen(*(values + 0));

	if (debug > 0) fprintf(stderr, "Allocate bind_param_is_null memory for %d.\n", (*(bind_param_length) == 0) ? 1 : 0);
	bind_param_is_null = malloc(sizeof(bool));
	if (bind_param_is_null == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_param_is_null memory.\n");
		exit(EXIT_FAILURE);
	}
	*(bind_param_is_null) = (*(bind_param_length) == 0) ? (bool)1 : (bool)0;

	bind_params_index++;
	(bind_params + bind_params_index)->buffer_type = fields[0].mst;
	(bind_params + bind_params_index)->buffer = bind_param_buffer;
	(bind_params + bind_params_index)->length = bind_param_length;
	(bind_params + bind_params_index)->is_null = bind_param_is_null;

	if (debug > 0) fprintf(stderr, "Bind the parameters.\n");
	mysql_stmt_bind_param(stmt, bind_params);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	if (debug > 0) fprintf(stderr, "Execute the prepared statement.\n");
	i = mysql_stmt_execute(stmt);
	if (debug > 0) fprintf(stderr, "i=%d\n", i);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	affected_rows = mysql_stmt_affected_rows(stmt);
	if (debug > 0) fprintf(stderr, "%ld row deleted.\n", affected_rows);

	printf("Status: 303 See other\n");
	printf("Location: /cgi-bin/select.cgi\n\n");
	fflush(stdout);

	if (debug > 0) fprintf(stderr, "Done.\n");
	fflush(stderr);
	fsync(2);

	if (debug > 0) fprintf(stderr, "Close the prepared statement:%p\n", stmt);
	mysql_stmt_close(stmt);
	if (debug > 0) fprintf(stderr, "Close the mysql connection:%p\n", mysql);
	mysql_close(mysql);
	fflush(stderr);

	return 0;
}