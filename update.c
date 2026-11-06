#include <ctype.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ghdb-cgi-bin.h"

/*
gcc update.c -o update.cgi -I/usr/include/mysql -lmysqlclient && chmod 755 update.cgi && sudo cp update.cgi /usr/lib/cgi-bin

cat Apple.txt | env CONTENT_LENGTH=`stat -c %s Apple.txt` ./update.cgi

*/

int main()
{
	int i;

	int key_values_length = 0;
	struct KEY_VALUE *key_values;

	char **values;
	int field_index;
	int values_index = 0;
	int values_length = 0;

	MYSQL *mysql = NULL;
	MYSQL_STMT *stmt = NULL;
	bool *bind_param_is_null;
	char *bind_param_buffer;
	int bind_params_index = 0;
	long unsigned *bind_param_length;
	MYSQL_BIND *bind_params;

	uint64_t affected_rows;

	char *bind_buffer;
	long unsigned *bind_length;
	bool *bind_is_null;

	/*
		Get the POST key value pairs
	*/
	key_values = get_post_data(&key_values_length);
	fprintf(stderr, "key_values_length=%d\n", key_values_length);
	if (key_values_length > 0)
	{		
		values = (char **)malloc(sizeof(char *) * NUM_FIELDS);
		for (i = 0; i < NUM_FIELDS; i++)
		{
			*(values + i) = (char *)malloc(sizeof(char) * 1);
			strcpy(*(values + i), "");
		}
		values_length = 0;
		for (i = 0; i < key_values_length; i++)
		{
			field_index = get_field_index((key_values + i)->key);
			fprintf(stderr, "field_index=%d\n", field_index);
			if (field_index > -1)
			{
				values_length++;
				strcpy(*(values + field_index), url_decode((key_values + i)->value)); 
				fprintf(stderr, "*(values + %d)=\"%s\"\n", field_index, *(values + field_index));
			} 
		}
	}	
	/*
		Get a connection to MySQL
	*/
	mysql = get_mysql_connection();
	if (mysql == NULL)
	{
		// TODO send a 500?
		exit(EXIT_FAILURE);
	}

	//fprintf(stderr, "Initialize a new INSERT prepared statement.\n");
	stmt = mysql_stmt_init(mysql);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}
	//fprintf(stderr, "New prepared statement: %p\n", stmt);

	//fprintf(stderr, "Prepare the statement.\n");
	i = mysql_stmt_prepare(stmt, ghdb_update, strlen(ghdb_update));
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	//fprintf(stderr, "Build the bind_param variable.\n");
	bind_params = (struct MYSQL_BIND *)malloc(sizeof(struct MYSQL_BIND) * NUM_FIELDS);
	if (bind_params == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_params memory.\n");
		exit(EXIT_FAILURE);
	}
	//fprintf(stderr, "Set the bind_param array to zeroes.\n");
	memset(bind_params, 0, sizeof(struct MYSQL_BIND) * NUM_FIELDS);
	fprintf(stderr, "bind_params=%p\n", bind_params);
	bind_params_index = -1;
	for (i = 1; i < NUM_FIELDS; i++) // start at 1 to skip plant_name, the primary key
	{
		fprintf(stderr, "Initialize the bind_params structure for %s.\n", fields[i].n);

		fprintf(stderr, "Allocate bind_param_buffer memory for \"%s\".\n", *(values + i));
		bind_param_buffer = malloc(strlen(*(values + i)) + 1);
		strcpy(bind_param_buffer, strlen(*(values + i)) == 0 ? "" : *(values + i));

		fprintf(stderr, "Allocate bind_param_length memory for %ld.\n", strlen(*(values + i)));
		bind_param_length = malloc(sizeof(unsigned long int));
		*(bind_param_length) = strlen(*(values + i));

		fprintf(stderr, "Allocate bind_param_is_null memory for %d.\n", (*(bind_param_length) == 0) ? 1 : 0);
		bind_param_is_null = malloc(sizeof(bool));
		*(bind_param_is_null) = (*(bind_param_length) == 0) ? (bool) 1 : (bool) 0;

		bind_params_index++;
		(bind_params + bind_params_index)->buffer_type = fields[i].mst;
		(bind_params + bind_params_index)->buffer = bind_param_buffer;
		(bind_params + bind_params_index)->length = bind_param_length;
		(bind_params + bind_params_index)->is_null = bind_param_is_null;
	}
	// now set the where clause
	fprintf(stderr, "Initialize the bind_params structure for %s.\n", fields[0].n);

	fprintf(stderr, "Allocate bind_param_buffer memory for %s.\n", *(values + 0));
	bind_param_buffer = malloc(strlen(*(values + 0)) + 1);
	strcpy(bind_param_buffer, *(values + 0));

	fprintf(stderr, "Allocate bind_param_length memory for %ld.\n", strlen(*(values + 0)));
	bind_param_length = malloc(sizeof(unsigned long int));
	*(bind_param_length) = strlen(*(values + 0));

	fprintf(stderr, "Allocate bind_param_is_null memory for %d.\n", (*(bind_param_length) == 0) ? 1 : 0);
	bind_param_is_null = malloc(sizeof(int));
	*(bind_param_is_null) = (*(bind_param_length) == 0) ? 1 : 0;

	bind_params_index++;
	(bind_params + bind_params_index)->buffer_type = fields[0].mst;
	(bind_params + bind_params_index)->buffer = bind_param_buffer;
	(bind_params + bind_params_index)->length = bind_param_length;
	(bind_params + bind_params_index)->is_null = bind_param_is_null;

	fprintf(stderr, "Bind the parameters.\n");
	mysql_stmt_bind_param(stmt, bind_params);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	//fprintf(stderr, "Bind the parameters.\n");
	mysql_stmt_bind_param(stmt, bind_params);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	//fprintf(stderr, "Execute the prepared statement.\n");
	i = mysql_stmt_execute(stmt);
	//fprintf(stderr, "i=%d\n", i);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	affected_rows = mysql_stmt_affected_rows(stmt);
	//fprintf(stderr, "%ld row updated.\n", affected_rows);

	//fprintf(stderr, "Close the prepared statement.\n");
	mysql_stmt_close(stmt);

	printf("Status: 303 See other\n");
	printf("Location: /cgi-bin/select.cgi\n\n");
	fflush(stdout);

	return 0;
}