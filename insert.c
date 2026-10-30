#include <ctype.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ghdb-cgi-bin.h"
#include "insert.h"

/*
	gcc insert.c -o insert.cgi -I/usr/include/mysql -lmysqlclient && chmod 755 insert.cgi && sudo cp insert.cgi /usr/lib/cgi-bin
*/

int main()
{
	char *content_length;
	char *key;
	char *token;
	char *value;
	char **values;
	char *buffer;
	int fi;
	int bl;
	int c = 0;
	int l = 0;
	int i;
	long unsigned lengths[NUM_FIELDS];
	char *mysql_user;
	char *mysql_host;
	char *mysql_pwd;
	char *mysql_db;
	int rc = 0;
	MYSQL *mysql = NULL;
	MYSQL_STMT *stmt = NULL;
	MYSQL_BIND bind[NUM_FIELDS];
	uint64_t affected_rows;
	char *bind_buffer;
	long unsigned *bind_length;
	bool *bind_is_null;

	content_length = getenv("CONTENT_LENGTH");
	//fprintf(stderr, "content_length=\"%s\"\n", content_length);
	if (content_length != NULL)
	{
		l = atoi(content_length);
		//fprintf(stderr, "content_length=%d\n", l);
	}

	buffer = malloc(l + 1);
	if (buffer == NULL)
	{
		//fprintf(stderr, "Failed to allocate %d bytes for buffer.\n", l + 1);
	}

	if (l > 0)
	{
		fgets(buffer, l + 1, stdin);
		//fprintf(stderr, "stdin=\"%s\"\n", buffer);
	}

	bl = strlen(buffer);
	if (bl > 0)
	{
		for (i = 0; i < bl; i++, c += (*(buffer + i) == '&') ? 1 : 0);
		values = malloc((sizeof(char *) * c));
		while ((token = strsep(&buffer, "&\n")))
		{
			//fprintf(stderr, "token=\"%s\"\n", token);
			while ((key = strsep(&token, "=")))
			{
				//fprintf(stderr, "key=\"%s\"\n", key);
				value = strsep(&token, "=");
				//fprintf(stderr, "value=\"%s\"\n", value);
				fi = field_index(key);
				//fprintf(stderr, "fi=%d\n", fi);
				if (fi > -1)
				{
					*(values + fi) = url_decode(value); 
				} 
			}
		}
	}
	//fprintf(stderr, "c=%d\n", c);
	for (i = 0; i < c; i++)
	{
		if (*(values + i) != NULL)
		{
			//fprintf(stderr, "values + %d = \"%s\"\n", i, *(values + i));
		}
	}

	//fprintf(stderr, "done.\n");

	free(buffer);

	mysql_user = malloc(31);
	mysql_host = malloc(31);
	mysql_pwd = malloc(31);
	mysql_db = malloc(31);

	strcpy(mysql_user, getenv("MYSQL_USER") != NULL ? getenv("MYSQL_USER") : "User not found.");
	strcpy(mysql_host, getenv("MYSQL_HOST") != NULL ? getenv("MYSQL_HOST") : "Host not found.");
	strcpy(mysql_pwd, getenv("MYSQL_PWD") != NULL ? getenv("MYSQL_PWD") : "Password not found.");
	strcpy(mysql_db, getenv("MYSQL_DB") != NULL ? getenv("MYSQL_DB") : "Database not found.");

	//fprintf(stderr, "mysql_user=\"%s\"\n", mysql_user);
	//fprintf(stderr, "mysql_host=\"%s\"\n", mysql_host);
	//fprintf(stderr, "mysql_db=\"%s\"\n", mysql_db);

	mysql = mysql_init(NULL);

	//fprintf(stderr, "mysql=%p\n", (void *)mysql);

	mysql_real_connect(mysql, mysql_host, mysql_user, mysql_pwd, mysql_db, 0, NULL, 0);
	if (mysql_errno(mysql) != 0)
	{
		fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(mysql));
		fflush(stdout);
		sleep(1);
		exit(EXIT_FAILURE);
	}

	//fprintf(stderr, "Initialize a new INSERT prepared statement.\n");
	stmt = mysql_stmt_init(mysql);
	if (mysql_stmt_errno(stmt) != 0)
	{
		//fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
	}
	//fprintf(stderr, "New prepared statement: %p\n", stmt);

	//fprintf(stderr, "Prepare the statement.\n");
	//sql = malloc(strlen(sql));
	//strcpy(sql , "insert into ghdb (plant_name) values (?)");
	// the SQL INSERT statement is defined in insert.h
	//fprintf(stderr, "sql=\"%s\"\n", sql);

	rc = mysql_stmt_prepare(stmt, sql, strlen(sql));
	if (mysql_stmt_errno(stmt) != 0)
	{
		//fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
	}

	//fprintf(stderr, "Build the bind variable.\n");
	//fprintf(stderr, "Set the bind array to zeroes.\n");
	memset(bind, 0, sizeof(bind));
	for (i = 0; i < c; i++)
	{
		////fprintf(stderr, "values + %d = \"%s\"\n", i, *(values + i));
		//plant_name = malloc(31);
		//strcpy(plant_name, "Strawberry");
		//plant_name_length = strlen(plant_name);

		//fprintf(stderr, "Initialize the bind structure for %s.\n", fields[i].n);

		bind_buffer = malloc(strlen(*(values + i)) + 1);
		strcpy(bind_buffer, *(values + i));

		bind_length = malloc(sizeof(unsigned long int));
		*(bind_length) = strlen(*(values + i));
		lengths[i] = *(bind_length);

		bind_is_null = malloc(sizeof(int));
		*(bind_is_null) = (*(bind_length) == 0) ? 1 : 0;

		//fprintf(stderr, "lengths[%d]=%ld\n", i, lengths[i]);
		bind[i].buffer_type = fields[i].mst;
		bind[i].buffer = bind_buffer;
		bind[i].length = bind_length;
		bind[i].is_null = bind_is_null;
	}

	//fprintf(stderr, "Bind the parameters.\n");
	mysql_stmt_bind_param(stmt, bind);
	if (mysql_stmt_errno(stmt) != 0)
	{
		//fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
	}

	//fprintf(stderr, "Execute the prepared statement.\n");
	rc = mysql_stmt_execute(stmt);
	//fprintf(stderr, "rc=%d\n", rc);
	if (mysql_stmt_errno(stmt) != 0)
	{
		//fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
	}

	affected_rows = mysql_stmt_affected_rows(stmt);
	//fprintf(stderr, "%ld row inserted.\n", affected_rows);

	//fprintf(stderr, "Close the prepared statement.\n");
	mysql_stmt_close(stmt);

	printf("Status: 303 See other\n");
	printf("Location: /cgi-bin/add.cgi?0\n\n");
	fflush(stdout);
	//sleep(1);

	return 0;
}