#include <ctype.h>
#include <fcntl.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ghdb-cgi-bin.h"
/*
gcc edit.c -o edit.cgi -I/usr/include/mysql -lmysqlclient && chmod 755 edit.cgi && sudo cp edit.cgi /usr/lib/cgi-bin

cat Apple.txt | env CONTENT_LENGTH=`stat -c %s Apple.txt` ./edit.cgi
*/

int main()
{
	int i = 0;

	int key_values_length = 0;
	struct KEY_VALUE *key_values;

	char **values;
	int field_index;
	int values_index = 0;
	int values_length = 0;

	char *order = " order by 1";
	char *sql;
	int order_length = strlen(order);
	int select_length = strlen(ghdb_select);
	int where_length = 0;

	MYSQL *mysql = NULL;
	MYSQL_STMT *stmt = NULL;
	bool *bind_param_is_null;
	char *bind_param_buffer;
	int bind_params_index = 0;
	long unsigned *bind_param_length;
	MYSQL_BIND *bind_params;

	MYSQL_BIND *bind_results;
	bool *bind_result_errors;
	bool *bind_result_is_nulls;
	char **bind_result_buffers;
	unsigned long *bind_result_lengths;

	char *row = NULL;
	int row_length = 0;

	char *file_buffer= NULL;
	char *file_name = "/var/www/html/ghdbform.html";
	char *pointer = NULL;
	FILE *file_pointer = NULL;

	int alt = 0;
	/*
		Initialize the results memory
	*/
	bind_result_errors = (bool *)malloc(sizeof(bool *) * NUM_FIELDS);
	for (i = 0; i < NUM_FIELDS; i++)
	{
		*(bind_result_errors + i) = (bool) 0;
	}

	bind_result_is_nulls = (bool *)malloc(sizeof(bool *) * NUM_FIELDS);
	for (i = 0; i < NUM_FIELDS; i++)
	{
		*(bind_result_is_nulls + i) = (bool) 0;
	}

	bind_result_buffers = (char **)malloc(sizeof(char *) * NUM_FIELDS);
	for (i = 0; i < NUM_FIELDS; i++)
	{
		*(bind_result_buffers + i) = (char *)malloc(sizeof(char) * 2001);
		strcpy(*(bind_result_buffers + i), "");
	}

	bind_result_lengths = (unsigned long *)malloc(sizeof(unsigned long *) * NUM_FIELDS);
	for (i = 0; i < NUM_FIELDS; i++)
	{
		*(bind_result_lengths + i) = (unsigned long) 0;
	}

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

	/*
		Create a prepared SELECT statement
	*/
	fprintf(stderr, "Initialize a new prepared SELECT statement.\n");
	stmt = mysql_stmt_init(mysql);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		// TODO send a 500?
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "New prepared statement: %p\n", stmt);

	fprintf(stderr, "Calculate the where clause length.\n");
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
	fprintf(stderr, "Where clause length=%d\n", where_length);
	sql = malloc(strlen(ghdb_select) + where_length + order_length + 6); // includes room for 1 = 1
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
					fprintf(stderr, "It's an equal statement.\n");
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
					fprintf(stderr, "It's a like statement.\n");
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
	fprintf(stderr, "sql=\"%s\"\n", sql);

	fprintf(stderr, "Prepare the statement.\n");

	i = mysql_stmt_prepare(stmt, sql, strlen(sql));
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
	}

	if (where_length > 0)
	{
		bind_params = (struct MYSQL_BIND *)malloc(sizeof(struct MYSQL_BIND *) * values_length);
		if (bind_params == NULL)
		{
			fprintf(stderr, "Failed to allocate bind_params memory.\n");
		}
		memset(bind_params, 0, sizeof(struct MYSQL_BIND *) * values_length);
		fprintf(stderr, "bind_params=%p\n", bind_params);
		bind_params_index = -1;
		for (i = 0; i < NUM_FIELDS; i++)
		{
			if (strlen(*(values + i)) > 0)
			{
				fprintf(stderr, "Initialize the bind_params structure for %s.\n", fields[i].n);

				fprintf(stderr, "Allocate bind_param_buffer memory for %s.\n", *(values + i));
				bind_param_buffer = malloc(strlen(*(values + i)) + 1);
				strcpy(bind_param_buffer, *(values + i));

				fprintf(stderr, "Allocate bind_param_length memory for %ld.\n", strlen(*(values + i)));
				bind_param_length = malloc(sizeof(unsigned long int));
				*(bind_param_length) = strlen(*(values + i));

				fprintf(stderr, "Allocate bind_param_is_null memory for %d.\n", (*(bind_param_length) == 0) ? 1 : 0);
				bind_param_is_null = malloc(sizeof(int));
				*(bind_param_is_null) = (*(bind_param_length) == 0) ? 1 : 0;

				//fprintf(stderr, "bind_param=%p\n", bind_param);
				bind_params_index++;
				(bind_params + bind_params_index)->buffer_type = fields[i].mst;
				fprintf(stderr, "buffer_type set.\n");
				(bind_params + bind_params_index)->buffer = bind_param_buffer;
				(bind_params + bind_params_index)->length = bind_param_length;
				(bind_params + bind_params_index)->is_null = bind_param_is_null;
			}
		}

		fprintf(stderr, "Bind the parameters.\n");
		mysql_stmt_bind_param(stmt, bind_params);
		if (mysql_stmt_errno(stmt) != 0)
		{
			fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
		}
	}


	fprintf(stderr, "Execute the prepared statement.\n");
	i = mysql_stmt_execute(stmt);
	fprintf(stderr, "i=%d\n", i);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
	}

	bind_results = (struct MYSQL_BIND *)malloc(sizeof(struct MYSQL_BIND *) * NUM_FIELDS);
	if (bind_results == NULL)
	{
		fprintf(stderr, "Failed to allocate bind_results memory.\n");
	}
	memset(bind_results, 0, sizeof(struct MYSQL_BIND *) * NUM_FIELDS);
	fprintf(stderr, "bind_results=%p\n", bind_results);

	for (i = 0; i < NUM_FIELDS; i++)
	{
		fprintf(stderr, "Initialize the bind_results structure for %s.\n", fields[i].n);
		(bind_results + i)->buffer_type = fields[i].mst;
		(bind_results + i)->buffer = *(bind_result_buffers + i); 
		(bind_results + i)->buffer_length = 2001; 
		(bind_results + i)->length = (bind_result_lengths + i);
		(bind_results + i)->is_null = (bind_result_is_nulls + i);
		(bind_results + i)->error = (bind_result_errors + i);
		fprintf(stderr, "End of bind result structure for %s.\n", fields[i].n);
	}

	fprintf(stderr, "Bind the parameters.\n");
	mysql_stmt_bind_result(stmt, bind_results);
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
	}
	else
	{
		fprintf(stderr, "Successfully bound the bind_results parameters\n");
	}

	fprintf(stderr, "\nFetch results\n");
	i = mysql_stmt_fetch(stmt);

	//if (i == 1 ...
	if (mysql_stmt_errno(stmt) != 0)
	{
		fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
	}

	if (i == MYSQL_NO_DATA)
	{
		fprintf(stderr, "MYSQL_NO_DATA, i=%d\n", i);
	}

	//printf("%s", row);
	fprintf(stderr, "Close the prepared statement.\n");
	(void) mysql_stmt_close(stmt);

	file_buffer = get_asset(file_name);

	for (i = 0; i < NUM_FIELDS; i++)
	{
		if (bind_result_is_nulls[i])
		{
			fprintf(stderr, "%s is NULL\n", fields[i].n);
			pointer = strrep(file_buffer, fields[i].cb, "");
			free(file_buffer);
			file_buffer = pointer;
		}
		else
		{
			fprintf(stderr, "%s = \"%s\"\n", fields[i].n, bind_result_buffers[i]);
			pointer = strrep(file_buffer, fields[i].cb, bind_result_buffers[i]);
			free(file_buffer);
			file_buffer = pointer;
		}
	}
	pointer = strrep(file_buffer, "{submit}", "Update");
	free(file_buffer);
	file_buffer = pointer;
	pointer = strrep(file_buffer, "action.cgi", "update.cgi");
	free(file_buffer);
	file_buffer = pointer;

	printf("Content-Type:text/html;charset=iso-8859-1\n\n");
	puts(file_buffer);
	free(file_buffer);
	fflush(stdout);;

	return 0;
}