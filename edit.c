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
gcc edit.c -o edit.cgi -I/usr/include/mysql -lmysqlclient && chmod 755 edit.cgi && sudo cp edit.cgi /usr/lib/cgi-bin

cat Apple.edit | env CONTENT_LENGTH=`stat -c %s Apple.edit` ./edit.cgi
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
	char *temp = NULL;

	char *order = " order by 1";
	char *sql = NULL;
	int order_length = strlen(order);
	int select_length = strlen(ghdb_select);
	int where_length = 0;

	MYSQL *mysql = NULL;
	MYSQL_STMT *stmt = NULL;

	MYSQL_BIND *bind_params = NULL;
	bool *bind_param_is_null = NULL;
	char *bind_param_buffer = NULL;
	long unsigned *bind_param_length = NULL;
	int bind_params_index = 0;

	MYSQL_BIND *bind_results = NULL;
	bool *bind_result_is_nulls = NULL;
	char **bind_result_buffers = NULL;
	unsigned long *bind_result_lengths = NULL;
	bool *bind_result_errors = NULL;

	char *file_buffer = NULL;
	char *file_name = "/var/www/html/ghdbform.html";
	char *pointer = NULL;
	char *temp_buffer = NULL;

	int alt = 0;
	
	debug = getenv("GHDB_DEBUG") == NULL ? 0 : atoi(getenv("GHDB_DEBUG"));

	/*
		Get the HTML template
	*/
	file_buffer = get_asset(file_name);

	/*
		Get a connection to MySQL
	*/
	mysql = get_mysql_connection();
	if (mysql == NULL)
	{
		dprintf(2, "%s: Failed to acquire a MySQL connection.\n", argv[0]);
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

	/*
		Create a prepared SELECT statement
	*/
	if (debug > 0) fprintf(stderr, "Initialize a new prepared SELECT statement.\n");
	stmt = mysql_stmt_init(mysql);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}
	if (debug > 0) fprintf(stderr, "New prepared statement: %p\n", stmt);

	if (debug > 0) fprintf(stderr, "Calculate the where clause length.\n");
	where_length = 0;
	if (values != NULL)
	{
		for (i = 0; i < NUM_FIELDS; i++)
		{
			if (strlen(*(values + i)) > 0)
			{
				if (strstr(*(values + i), "%") == NULL)
				{
					// it's an equal statement
					// the column name: strlen(fields[i].n)
					// space between the operator: 1
					// equal operator: 1
					// space between the parameter: 1
					// the parameter place holder, ?: 1
					// where_length > 0 means there's an " and ": 5
					where_length += (where_length == 0 ? 0 : 5) + strlen(fields[i].n) + 4;
				}
				else
				{
					// it's a like statement
					// the column name: strlen(fields[i].n)
					// space between the operator: 1
					// like operator: 4
					// space between the parameter: 1
					// the parameter place holder, ?: 1
					// where_length > 0 means there's an " and ": 5
					where_length += (where_length == 0 ? 0 : 5) + strlen(fields[i].n) + 7;
				}
			}
		}
	}
	if (debug > 0) fprintf(stderr, "Where clause length=%d\n", where_length);

	sql = malloc(strlen(ghdb_select) + where_length + order_length + 6); // includes room for 1 = 1
	if (sql == NULL)
	{
		fprintf(stderr, "Failed to allocate sql memory.\n");
		exit(EXIT_FAILURE);
	}
	strcpy(sql, ghdb_select);

	if (where_length > 0)
	{
		values_index = -1;
		for (i = 0; i < NUM_FIELDS; i++)
		{
			if (strlen(*(values + i)) > 0)
			{
				if (strstr(*(values + i), "%") == NULL)
				{
					if (debug > 0) fprintf(stderr, "It's an equal statement.\n");
					// the column name: strlen(fields[i].n)
					// space between the operator: 1
					// equal operator: 1
					// space between the parameter: 1
					// the parameter place holder, ?: 1
					// where_length > 0 means there's an " and ": 5
					values_index++;
					if (values_index > 0)
					{
						strcat(sql, " and ");
					} 
					strcat(sql, fields[i].n);
					strcat(sql, " = ?");
				}
				else
				{
					if (debug > 0) fprintf(stderr, "It's a like statement.\n");
					// the column name: strlen(fields[i].n)
					// space between the operator: 1
					// like operator: 4
					// space between the parameter: 1
					// the parameter place holder, ?: 1
					// where_length > 0 means there's an " and ": 5
					values_index++;
					if (values_index > 0)
					{
						strcat(sql, " and ");
					} 
					strcat(sql, fields[i].n);
					strcat(sql, " like ?");
				}
			}
		}
	}
	else
	{
		strcat(sql, "1 = 1");
	}
	strcat(sql, order);
	if (debug > 0) dprintf(2, "sql=\"%s\"\n", sql);

	if (debug > 0) fprintf(stderr, "Prepare the statement.\n");

	i = mysql_stmt_prepare(stmt, sql, strlen(sql));
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	if (where_length > 0)
	{
		bind_params = (struct MYSQL_BIND *)malloc(sizeof(struct MYSQL_BIND) * values_length);
		if (bind_params == NULL)
		{
			fprintf(stderr, "Failed to allocate bind_params memory.\n");
		}
		memset(bind_params, 0, sizeof(struct MYSQL_BIND) * values_length);
		if (debug > 0) fprintf(stderr, "bind_params=%p\n", bind_params);
		bind_params_index = -1;

		for (i = 0; i < NUM_FIELDS; i++)
		{
			if (strlen(*(values + i)) > 0)
			{
				if (debug > 0) fprintf(stderr, "Initialize the bind_params structure for %s.\n", fields[i].n);

				if (debug > 0) fprintf(stderr, "Allocate bind_param_buffer memory for %s.\n", *(values + i));
				bind_param_buffer = malloc(strlen(*(values + i)) + 1);
				if (bind_param_buffer == NULL)
				{
					fprintf(stderr, "Failed to allocate bind_param_buffer memory.\n");
					exit(EXIT_FAILURE);
				}
				strcpy(bind_param_buffer, *(values + i));

				if (debug > 0) fprintf(stderr, "Allocate bind_param_length memory for %ld.\n", strlen(*(values + i)));
				bind_param_length = malloc(sizeof(unsigned long int));
				if (bind_param_length == NULL)
				{
					fprintf(stderr, "Failed to allocate bind_param_length memory.\n");
					exit(EXIT_FAILURE);
				}
				*(bind_param_length) = strlen(*(values + i));

				if (debug > 0) fprintf(stderr, "Allocate bind_param_is_null memory for %d.\n", (*(bind_param_length) == 0) ? 1 : 0);
				bind_param_is_null = malloc(sizeof(int));
				if (bind_param_is_null == NULL)
				{
					fprintf(stderr, "Failed to allocate bind_param_is_null memory.\n");
					exit(EXIT_FAILURE);
				}
				*(bind_param_is_null) = (*(bind_param_length) == 0) ? 1 : 0;

				bind_params_index++;

				(bind_params + bind_params_index)->buffer_type = fields[i].mst;
				if (debug > 0) fprintf(stderr, "buffer_type set.\n");
				(bind_params + bind_params_index)->buffer = bind_param_buffer;
				(bind_params + bind_params_index)->length = bind_param_length;
				(bind_params + bind_params_index)->is_null = bind_param_is_null;
			}
		}

		if (debug > 0) fprintf(stderr, "Bind the parameters.\n");
		mysql_stmt_bind_param(stmt, bind_params);
		if (mysql_stmt_errno(stmt) != 0)
		{
			fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
			exit(EXIT_FAILURE);
		}
	}

	if (debug > 0) fprintf(stderr, "Execute the prepared statement.\n");
	i = mysql_stmt_execute(stmt);
	if (debug > 0) fprintf(stderr, "i=%d\n", i);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	/*
		Initialize the results memory
	*/
	if (debug > 0) fprintf(stderr, "Initialize the bind result arrays.\n");
	bind_result_errors = (bool *)malloc(sizeof(bool *) * NUM_FIELDS);
	if (bind_result_errors == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_result_errors memory.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < NUM_FIELDS; i++)
	{
		*(bind_result_errors + i) = (bool) 0;
	}

	bind_result_is_nulls = (bool *)malloc(sizeof(bool *) * NUM_FIELDS);
	if (bind_result_is_nulls == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_result_is_nulls memory.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < NUM_FIELDS; i++)
	{
		*(bind_result_is_nulls + i) = (bool) 0;
	}

	bind_result_buffers = (char **)malloc(sizeof(char *) * NUM_FIELDS);
	if (bind_result_buffers == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_result_buffers memory.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < NUM_FIELDS; i++)
	{
		*(bind_result_buffers + i) = (char *)malloc(sizeof(char) * 2001);
		if (*(bind_result_buffers + i) == NULL)
		{
			fprintf(stderr, "Failed to allocate bind_result_buffer memory.\n");
			exit(EXIT_FAILURE);
		}
		strcpy(*(bind_result_buffers + i), "");
	}

	bind_result_lengths = (unsigned long *)malloc(sizeof(unsigned long *) * NUM_FIELDS);
	if (bind_result_lengths == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_result_lengths memory.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < NUM_FIELDS; i++)
	{
		*(bind_result_lengths + i) = (unsigned long) 0;
	}

	if (debug > 0) fprintf(stderr, "Initialize the bind results.\n");
	bind_results = (struct MYSQL_BIND *)malloc(sizeof(struct MYSQL_BIND) * NUM_FIELDS);
	if (bind_results == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_results memory.\n");
		exit(EXIT_FAILURE);
	}
	memset(bind_results, 0, sizeof(struct MYSQL_BIND) * NUM_FIELDS);
	if (debug > 0) fprintf(stderr, "bind_results=%p\n", bind_results);

	for (i = 0; i < NUM_FIELDS; i++)
	{
		if (debug > 0) fprintf(stderr, "Initialize the bind_results structure for %s.\n", fields[i].n);
		(bind_results + i)->buffer_type = fields[i].mst;
		(bind_results + i)->buffer = *(bind_result_buffers + i); 
		(bind_results + i)->buffer_length = 2000; 
		(bind_results + i)->length = (bind_result_lengths + i);
		(bind_results + i)->is_null = (bind_result_is_nulls + i);
		(bind_results + i)->error = (bind_result_errors + i);
		if (debug > 0) fprintf(stderr, "End of bind result structure for %s.\n", fields[i].n);
	}

	if (debug > 0) fprintf(stderr, "Bind the parameters.\n");
	mysql_stmt_bind_result(stmt, bind_results);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}
	else
	{
		if (debug > 0) fprintf(stderr, "Successfully bound the bind_results parameters\n");
	}

	if (debug > 0) fprintf(stderr, "\nFetch results\n");
	i = mysql_stmt_fetch(stmt);

	//if (i == 1 ...
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		exit(EXIT_FAILURE);
	}

	if (i == MYSQL_NO_DATA)
	{
		if (debug > 0) fprintf(stderr, "MYSQL_NO_DATA, i=%d\n", i);
	}

	// Now send the form
	temp_buffer = malloc(strlen(file_buffer) + 1);
	if (temp_buffer == NULL) 
	{
		printf("Failed to allocate %ld bytes of memory for temp_buffer.\n", strlen(file_buffer) + 1);
		exit(EXIT_FAILURE);
	}
	strcpy(temp_buffer, file_buffer);

	for (i = 0; i < NUM_FIELDS; i++)
	{
		if (bind_result_is_nulls[i])
		{
			if (debug > 0) fprintf(stderr, "%s is NULL\n", fields[i].n);
			pointer = strrep(temp_buffer, fields[i].cb, "");
			free(temp_buffer);
			temp_buffer = pointer;
		}
		else
		{
			if (debug > 0) fprintf(stderr, "%s = \"%s\"\n", fields[i].n, bind_result_buffers[i]);
			temp = html_encode(bind_result_buffers[i]);
			pointer = strrep(temp_buffer, fields[i].cb, temp);
			free(temp);
			free(temp_buffer);
			temp_buffer = pointer;
		}
	}
	pointer = strrep(temp_buffer, "{submit}", "Update");
	free(temp_buffer);
	temp_buffer = pointer;
	pointer = strrep(temp_buffer, "action.cgi", "update.cgi");
	free(temp_buffer);
	temp_buffer = pointer;

	printf("Content-Type:text/html;charset=iso-8859-1\n\n");
	puts(temp_buffer);
	fflush(stdout);
	free(temp_buffer);

	if (debug > 0) fprintf(stderr, "Close the prepared statement:%p\n", stmt);
	mysql_stmt_close(stmt);

	if (debug > 0) fprintf(stderr, "Done.\n");
	fflush(stderr);
	fsync(2);

	if (debug > 0) fprintf(stderr, "Close the mysql connection:%p\n", mysql);
	mysql_close(mysql);
	fflush(stderr);
	
	return 0;
}