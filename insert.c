#include <ctype.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ghdb-cgi-bin.h"

/*
	gcc insert.c -o insert.cgi -I/usr/include/mysql -lmysqlclient && chmod 755 insert.cgi && sudo cp insert.cgi /usr/lib/cgi-bin
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

	long unsigned lengths[NUM_FIELDS];

	MYSQL *mysql = NULL;
	MYSQL_STMT *stmt = NULL;
	MYSQL_BIND bind[NUM_FIELDS];
	char *bind_buffer;
	long unsigned *bind_length;
	bool *bind_is_null;
	uint64_t affected_rows;

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
	i = mysql_stmt_prepare(stmt, ghdb_insert, strlen(ghdb_insert));
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	//fprintf(stderr, "Build the bind variable.\n");
	//fprintf(stderr, "Set the bind array to zeroes.\n");
	memset(bind, 0, sizeof(bind));
	for (i = 0; i < NUM_FIELDS; i++)
	{
		//fprintf(stderr, "Initialize the bind structure for %s.\n", fields[i].n);
		bind_buffer = malloc(strlen(*(values + i)) + 1);
		strcpy(bind_buffer, *(values + i));

		bind_length = malloc(sizeof(unsigned long int));
		*(bind_length) = strlen(*(values + i));
		lengths[i] = *(bind_length);

		bind_is_null = malloc(sizeof(int));
		*(bind_is_null) = (*(bind_length) == 0) ? 1 : 0;

		bind[i].buffer_type = fields[i].mst;
		bind[i].buffer = bind_buffer;
		bind[i].length = bind_length;
		bind[i].is_null = bind_is_null;
	}

	//fprintf(stderr, "Bind the parameters.\n");
	mysql_stmt_bind_param(stmt, bind);
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
	//fprintf(stderr, "%ld row inserted.\n", affected_rows);

	//fprintf(stderr, "Close the prepared statement.\n");
	mysql_stmt_close(stmt);

	printf("Status: 303 See other\n");
	printf("Location: /cgi-bin/add.cgi?0\n\n");
	fflush(stdout);

	return 0;
}