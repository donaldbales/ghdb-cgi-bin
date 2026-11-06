#ifndef __GHDB_CGI_BIN_H
#define __GHDB_CGI_BIN_H

#define PLANT_NAME_FIELD               0
#define LATIN_NAME_FIELD               1
#define HEIGHT_FIELD                   2
#define WIDTH_FIELD                    3
#define PLANTING_DEPTH_FIELD           4
#define SEED_SIZE_FIELD                5
#define SEED_NEED_LIGHT_FIELD          6
#define SEED_SCARIFICATION_FIELD       7
#define PH_FIELD                       8
#define EC_FIELD                       9
#define DAY_LIGHT_INTERVAL_FIELD      10
#define PHOTOPERIOD_HOURS_FIELD       11
#define LIGHT_LOWER_FIELD             12
#define LIGHT_OPTIMAL_FIELD           13
#define LIGHT_UPPER_FIELD             14
#define NITROGEN_LOWER_FIELD          15
#define NITROGEN_OPTIMAL_FIELD        16
#define NITROGEN_UPPER_FIELD          17
#define PHOSPHORUS_LOWER_FIELD        18
#define PHOSPHORUS_OPTIMAL_FIELD      19
#define PHOSPHORUS_UPPER_FIELD        20
#define POTASSIUM_LOWER_FIELD         21
#define POTASSIUM_OPTIMAL_FIELD       22
#define POTASSIUM_UPPER_FIELD         23
#define GERMINATION_LOWER_FIELD       24
#define GERMINATION_NORMAL_FIELD      25
#define GERMINATION_UPPER_FIELD       26
#define TRANSPLANTING_LOWER_FIELD     27
#define TRANSPLANTING_OPTIMAL_FIELD   28
#define TRANSPLANTING_UPPER_FIELD     29
#define MATURITY_LOWER_FIELD          30
#define MATURITY_OPTIMAL_FIELD        31
#define MATURITY_UPPER_FIELD          32
#define FROST_TOLERANCE_FIELD         33
#define FLOWERING_FIELD               34
#define POLLINATION_PRIMARY_FIELD     35
#define POLLINATION_SECONDARY_FIELD   36

#define PLANT_NAME_LENGTH             30
#define LATIN_NAME_LENGTH             30
#define HEIGHT_LENGTH                  5
#define WIDTH_LENGTH                   5
#define PLANTING_DEPTH_LENGTH          5
#define SEED_SIZE_LENGTH               5
#define SEED_NEED_LIGHT_LENGTH         3
#define SEED_SCARIFICATION_LENGTH      3
#define PH_LENGTH                      5
#define EC_LENGTH                      5
#define DAY_LIGHT_INTERVAL_LENGTH      5
#define PHOTOPERIOD_HOURS_LENGTH       5
#define LIGHT_LOWER_LENGTH             5
#define LIGHT_OPTIMAL_LENGTH           5
#define LIGHT_UPPER_LENGTH             5
#define NITROGEN_LOWER_LENGTH          5
#define NITROGEN_OPTIMAL_LENGTH        5
#define NITROGEN_UPPER_LENGTH          5
#define PHOSPHORUS_LOWER_LENGTH        5
#define PHOSPHORUS_OPTIMAL_LENGTH      5
#define PHOSPHORUS_UPPER_LENGTH        5
#define POTASSIUM_LOWER_LENGTH         5
#define POTASSIUM_OPTIMAL_LENGTH       5
#define POTASSIUM_UPPER_LENGTH         5
#define GERMINATION_LOWER_LENGTH       5
#define GERMINATION_NORMAL_LENGTH      5
#define GERMINATION_UPPER_LENGTH       5
#define TRANSPLANTING_LOWER_LENGTH     5
#define TRANSPLANTING_OPTIMAL_LENGTH   5
#define TRANSPLANTING_UPPER_LENGTH     5
#define MATURITY_LOWER_LENGTH          5
#define MATURITY_OPTIMAL_LENGTH        5
#define MATURITY_UPPER_LENGTH          5
#define FROST_TOLERANCE_LENGTH         3
#define FLOWERING_LENGTH               5
#define POLLINATION_PRIMARY_LENGTH    30
#define POLLINATION_SECONDARY_LENGTH  30

#define NUM_FIELDS 37

struct FIELD
{
	char n[31];
	char cb[33];
	enum_field_types mst;
};

struct KEY_VALUE
{
	char *key;
	char *value;
};

struct FIELD fields[NUM_FIELDS] = 
{
	{ "plant_name", "{plant_name}", MYSQL_TYPE_STRING },
	{ "latin_name", "{latin_name}", MYSQL_TYPE_STRING },
	{ "height", "{height}", MYSQL_TYPE_STRING },
	{ "width", "{width}", MYSQL_TYPE_STRING },
	{ "planting_depth", "{planting_depth}", MYSQL_TYPE_STRING },
	{ "seed_size", "{seed_size}", MYSQL_TYPE_STRING },
	{ "seed_need_light", "{seed_need_light}", MYSQL_TYPE_STRING },
	{ "seed_scarification", "{seed_scarification}", MYSQL_TYPE_STRING },
	{ "ph", "{ph}", MYSQL_TYPE_STRING },
	{ "ec", "{ec}", MYSQL_TYPE_STRING },
	{ "day_light_interval", "{day_light_interval}", MYSQL_TYPE_STRING },
	{ "photoperiod_hours", "{photoperiod_hours}", MYSQL_TYPE_STRING },
	{ "light_lower", "{light_lower}", MYSQL_TYPE_STRING },
	{ "light_optimal", "{light_optimal}", MYSQL_TYPE_STRING },
	{ "light_upper", "{light_upper}", MYSQL_TYPE_STRING },
	{ "nitrogen_lower", "{nitrogen_lower}", MYSQL_TYPE_STRING },
	{ "nitrogen_optimal", "{nitrogen_optimal}", MYSQL_TYPE_STRING },
	{ "nitrogen_upper", "{nitrogen_upper}", MYSQL_TYPE_STRING },
	{ "phosphorus_lower", "{phosphorus_lower}", MYSQL_TYPE_STRING },
	{ "phosphorus_optimal", "{phosphorus_optimal}", MYSQL_TYPE_STRING },
	{ "phosphorus_upper", "{phosphorus_upper}", MYSQL_TYPE_STRING },
	{ "potassium_lower", "{potassium_lower}", MYSQL_TYPE_STRING },
	{ "potassium_optimal", "{potassium_optimal}", MYSQL_TYPE_STRING },
	{ "potassium_upper", "{potassium_upper}", MYSQL_TYPE_STRING },
	{ "germination_lower", "{germination_lower}", MYSQL_TYPE_STRING },
	{ "germination_normal", "{germination_normal}", MYSQL_TYPE_STRING },
	{ "germination_upper", "{germination_upper}", MYSQL_TYPE_STRING },
	{ "transplanting_lower", "{transplanting_lower}", MYSQL_TYPE_STRING },
	{ "transplanting_optimal", "{transplanting_optimal}", MYSQL_TYPE_STRING },
	{ "transplanting_upper", "{transplanting_upper}", MYSQL_TYPE_STRING },
	{ "maturity_lower", "{maturity_lower}", MYSQL_TYPE_STRING },
	{ "maturity_optimal", "{maturity_optimal}", MYSQL_TYPE_STRING },
	{ "maturity_upper", "{maturity_upper}", MYSQL_TYPE_STRING },
	{ "frost_tolerance", "{frost_tolerance}", MYSQL_TYPE_STRING },
	{ "flowering", "{flowering}", MYSQL_TYPE_STRING },
	{ "pollination_primary", "{pollination_primary}", MYSQL_TYPE_STRING },
	{ "pollination_secondary", "{pollination_secondary}", MYSQL_TYPE_STRING }
};

char *ghdb_delete = 
	"delete from ghdb "
		"where plant_name = ?";

char *ghdb_insert = 
	"insert into ghdb ( "
		"plant_name, "
		"latin_name, "
		"height, "
		"width, "
		"planting_depth, "
		"seed_size, "
		"seed_need_light, "
		"seed_scarification, "
		"ph, "
		"ec, "
		"day_light_interval, "
		"photoperiod_hours, "
		"light_lower, "
		"light_optimal, "
		"light_upper, "
		"nitrogen_lower, "
		"nitrogen_optimal, "
		"nitrogen_upper, "
		"phosphorus_lower, "
		"phosphorus_optimal, "
		"phosphorus_upper, "
		"potassium_lower, "
		"potassium_optimal, "
		"potassium_upper, "
		"germination_lower, "
		"germination_normal, "
		"germination_upper, "
		"transplanting_lower, "
		"transplanting_optimal, "
		"transplanting_upper, "
		"maturity_lower, "
		"maturity_optimal, "
		"maturity_upper, "
		"frost_tolerance, "
		"flowering, "
		"pollination_primary, "
		"pollination_secondary) "
	"values ( "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?, "
		"?)";

char *ghdb_select = 
	"select "
		"plant_name, "
		"latin_name, "
		"height, "
		"width, "
		"planting_depth, "
		"seed_size, "
		"seed_need_light, "
		"seed_scarification, "
		"ph, "
		"ec, "
		"day_light_interval, "
		"photoperiod_hours, "
		"light_lower, "
		"light_optimal, "
		"light_upper, "
		"nitrogen_lower, "
		"nitrogen_optimal, "
		"nitrogen_upper, "
		"phosphorus_lower, "
		"phosphorus_optimal, "
		"phosphorus_upper, "
		"potassium_lower, "
		"potassium_optimal, "
		"potassium_upper, "
		"germination_lower, "
		"germination_normal, "
		"germination_upper, "
		"transplanting_lower, "
		"transplanting_optimal, "
		"transplanting_upper, "
		"maturity_lower, "
		"maturity_optimal, "
		"maturity_upper, "
		"frost_tolerance, "
		"flowering, "
		"pollination_primary, "
		"pollination_secondary "
	"from ghdb where ";

char *ghdb_update = 
	"update ghdb set "
		"latin_name = ?, "
		"height = ?, "
		"width = ?, "
		"planting_depth = ?, "
		"seed_size = ?, "
		"seed_need_light = ?, "
		"seed_scarification = ?, "
		"ph = ?, "
		"ec = ?, "
		"day_light_interval = ?, "
		"photoperiod_hours = ?, "
		"light_lower = ?, "
		"light_optimal = ?, "
		"light_upper = ?, "
		"nitrogen_lower = ?, "
		"nitrogen_optimal = ?, "
		"nitrogen_upper = ?, "
		"phosphorus_lower = ?, "
		"phosphorus_optimal = ?, "
		"phosphorus_upper = ?, "
		"potassium_lower = ?, "
		"potassium_optimal = ?, "
		"potassium_upper = ?, "
		"germination_lower = ?, "
		"germination_normal = ?, "
		"germination_upper = ?, "
		"transplanting_lower = ?, "
		"transplanting_optimal = ?, "
		"transplanting_upper = ?, "
		"maturity_lower = ?, "
		"maturity_optimal = ?, "
		"maturity_upper = ?, "
		"frost_tolerance = ?, "
		"flowering = ?, "
		"pollination_primary = ?, "
		"pollination_secondary = ? "
	"where plant_name = ?";

char * html_encode(char *string)
{
	int string_length = strlen(string);
	char c;
	char *p;
	char *result = NULL;
	int result_length = string_length;
	int i = 0;
	for (i=0; i<string_length; i++)
	{
		c = *(string + i);
		//printf("%c\n", c);
		switch (c)
		{
			case '<':
				result_length += 3;
				break;
			case '>':
				result_length += 3;
				break;
			case '&':
				result_length += 4;
				break;
			case '"':
				result_length += 5;
				break;
			case '\'':
				result_length += 5;
				break;
		}
	}
	result = malloc(result_length + 1);
	p = result;
	for (i=0; i<string_length; i++)
	{
		c = *(string + i);
		//printf("%c\n", c);
		switch (c)
		{
			case '<':
				strcat(p, "&lt;");
				p += 4;
				break;
			case '>':
				strcat(p, "&gt;");
				p += 4;
				break;
			case '&':
				strcat(p, "&amp;");
				p += 5;
				break;
			case '"':
				strcat(p, "&quot;");
				p += 6;
				break;
			case '\'':
				strcat(p, "&apos;");
				p += 6;
				break;
			default:
				*p = c;
				p += 1;
				break;
		}
	}

	return result;
}

char * strrep(char *string, const char *search, const char *replace)
{
	int length = 0;
	int offset = 0;
	int replace_length = strlen(replace);
	int search_length = strlen(search);
	int string_length = strlen(string);
	char *p;
	int occurs = 0;
	for (p = string; (p = strstr(p, search)) != NULL; p += search_length, occurs++);
	int result_length = string_length - (search_length * occurs) + (replace_length * occurs) + 1;
	char *result = malloc(result_length);
	if (result == NULL) 
	{
		printf("Failed to allocate %d bytes of memory for result.\n", result_length);
	}
	char *buffer = malloc(string_length + 1);
	if (buffer == NULL) 
	{
		printf("Failed to allocate %d bytes of memory for buffer.\n", string_length);
	}
	char *b = buffer;
	//printf("string_length=%d\n", string_length);
	//printf("result_length=%d\n", result_length);
	//printf("string=\"%s\"\n", string);
	//printf("search=\"%s\"\n", search);
	//printf("replace=\"%s\"\n", replace);

	strcpy(buffer, string);
	while ((p = strstr(buffer, search)) != NULL) {
		//printf("offset1=%d\n", offset);
		length = p - buffer;
		//printf("length=%d\n", length);
		strncpy(result + offset, buffer, length);
		*(result + offset + length) = '\0';
		//printf("result1=\"%s\"\n", result);
		offset += length;
		//printf("offset2=%d\n", offset);
		strcpy(result + offset, replace);
		*(result + offset + result_length) = '\0';
		//printf("result2=\"%s\"\n", result);
		offset += replace_length;
		//printf("offset3=%d\n", offset);
		strcpy(buffer, p + search_length);
		//printf("buffer=\"%s\"\n", buffer);
	}
	strcpy(result + offset, buffer);
	
	free(b);
	//printf("strlen(result)=%d\n", strlen(result));
	return result;
}

char * url_decode(char *string)
{
	int string_length = strlen(string);
	char c;
	char c1;
	char c2;
	char x2i[3];
	char *p;
	char *result = NULL;
	int result_length = string_length;
	int i = 0;
	for (i=0; i<string_length; i++)
	{
		c = *(string + i);
		//printf("%c\n", c);
		switch (c)
		{
			case '%':
				if ((i + 2) < string_length)
				{
					c1 = *(string + i + 1);
					c2 = *(string + i + 2);
					if (isxdigit(c1) &&
						isxdigit(c2))
					{
						result_length += 2;
					}
				}
				break;
			case '+':
				break;
		}
	}
	result = malloc(result_length + 1);
	p = result;
	for (i=0; i<string_length; i++)
	{
		c = *(string + i);
		//printf("%c\n", c);
		switch (c)
		{
			case '%':
				if ((i + 2) < string_length)
				{
					c1 = *(string + i + 1);
					c2 = *(string + i + 2);
					if (isxdigit(c1) &&
						isxdigit(c2))
					{
						x2i[0] = c1;
						x2i[1] = c2;
						x2i[2] = '\0';
						*p = (char) strtol(x2i, NULL, 16);
						p += 1;
						i += 2;
					}
				}
				break;
			case '+':
				*p = ' ';
				p += 1;
				break;
			default:
				*p = c;
				p += 1;
				break;
		}
	}

	return result;
}

char * get_asset(char *file_name)
{
	int i = 0;

	struct stat file_status;

	char *file_buffer= NULL;
	char *pointer = NULL;
	FILE *file_pointer = NULL;
	size_t bytes_read = 0;

	i = stat(file_name, &file_status);
	if (i == 0) 
	{
		file_buffer = malloc(file_status.st_size);
		if (file_buffer == NULL) 
		{
			fprintf(stderr, "Failed to allocate %ld bytes of memory for file_buffer.\n", file_status.st_size);
			exit(EXIT_FAILURE);
		}
		//fprintf(strerr, "file_status.st_size=%lu\n", file_status.st_size);
	}

	file_pointer = fopen(file_name, "r");
	if (file_pointer)
	{
		bytes_read = fread(file_buffer, 1, file_status.st_size, file_pointer);
		//fprintf(stderr, "bytes_read=%lu\n", bytes_read);
		if (bytes_read != file_status.st_size)
		{
			fprintf(stderr, "Didn't read the whole file.\n");
			exit(EXIT_FAILURE);
		}
	}
	fclose(file_pointer);

	return file_buffer;
}

int get_field_index(char *name)
{
	for (int i = 0; i < NUM_FIELDS; i++)
	{
		if (strcmp(fields[i].n, name) == 0)
		{
			return i;
		}
	}

	return -1;
}

MYSQL * get_mysql_connection()
{
	char *mysql_user;
	char *mysql_host;
	char *mysql_pwd;
	char *mysql_db;
	char *sql;
	int rc = 0;
	MYSQL *mysql = NULL;

	// allocate for max sizes
	mysql_user = malloc(32 + 1);
	mysql_host = malloc(255 + 1);
	mysql_pwd = malloc(32 + 1);
	mysql_db = malloc(64 + 1);

	strcpy(mysql_user, getenv("MYSQL_USER") != NULL ? getenv("MYSQL_USER") : "User not key_values_length.");
	strcpy(mysql_host, getenv("MYSQL_HOST") != NULL ? getenv("MYSQL_HOST") : "Host not key_values_length.");
	strcpy(mysql_pwd, getenv("MYSQL_PWD") != NULL ? getenv("MYSQL_PWD") : "Password not key_values_length.");
	strcpy(mysql_db, getenv("MYSQL_DB") != NULL ? getenv("MYSQL_DB") : "Database not key_values_length.");

	fprintf(stderr, "mysql_user=\"%s\"\n", mysql_user);
	fprintf(stderr, "mysql_host=\"%s\"\n", mysql_host);
	fprintf(stderr, "mysql_db=\"%s\"\n", mysql_db);

	mysql = mysql_init(NULL);
	fprintf(stderr, "mysql=%p\n", (void *)mysql);

	mysql_real_connect(mysql, mysql_host, mysql_user, mysql_pwd, mysql_db, 0, NULL, 0);
	if (mysql_errno(mysql) != 0)
	{
		fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(mysql));
		mysql = NULL;
	}

	return mysql;
}

/*
	Get the POST key value pairs
*/
struct KEY_VALUE * get_post_data(int *key_values_length)
{
	char *buffer = NULL;
	char *content_length_string = NULL;
	char *token = NULL;
	char *key = NULL;
	char *value = NULL;
	int buffer_length;
	int content_length = 0;
	int key_values_found = 0;
	int key_values_index = 0;
	int i = 0;
	struct KEY_VALUE *key_values = NULL;

	content_length_string = getenv("CONTENT_LENGTH");
	fprintf(stderr, "content_length_string=\"%s\"\n", content_length_string);
	if (content_length_string != NULL)
	{
		content_length = atoi(content_length_string);
		fprintf(stderr, "content_length_string=%d\n", content_length);
	}

	buffer = malloc(content_length + 1);
	if (buffer == NULL)
	{
		fprintf(stderr, "Failed to allocate %d bytes for content buffer.\n", content_length + 1);

	}

	if (content_length > 0)
	{
		fgets(buffer, content_length + 1, stdin);
		fprintf(stderr, "buffer=\"%s\"\n", buffer);
	}

	buffer_length = strlen(buffer);
	if (buffer_length > 0)
	{
		fprintf(stderr, "buffer_length=%d\n", buffer_length);
		for (i = 0, key_values_found = 1; i < buffer_length; i++, key_values_found += (((char)*(buffer + i)) == '&') ? 1 : 0);
		fprintf(stderr, "key_values_found=%d\n", key_values_found);
		if (key_values_found > 0)
		{
			key_values = malloc((sizeof(struct KEY_VALUE) * key_values_found));
			memset(key_values, 0, sizeof(struct KEY_VALUE) * key_values_found);
			key_values_index = -1;
			while ((token = strsep(&buffer, "&\n"))) // the \n is a separator
			{
				fprintf(stderr, "token=\"%s\"\n", token);
				if (strlen(token) > 0)
				{
					while ((key = strsep(&token, "=")))
					{
						fprintf(stderr, "key=\"%s\"\n", key);
						value = strsep(&token, "=");
						fprintf(stderr, "value=\"%s\"\n", value);
						key_values_index++;
						/* this works, but's not how they say you should code it
						(key_values + key_value_index)->key = malloc(sizeof(char *));
						(key_values + key_value_index)->key = key;
						(key_values + key_value_index)->value = malloc(sizeof(char *));
						(key_values + key_value_index)->value = value;
						*/
						(key_values + key_values_index)->key = malloc(strlen(key) + 1);
						strcpy((key_values + key_values_index)->key, key);
						(key_values + key_values_index)->value = malloc(strlen(value) + 1);
						strcpy((key_values + key_values_index)->value, value);
						/*
						printf("key_values=%p\n", key_values);
						printf("(key_values + key_value_index)=%p\n", (key_values + key_value_index));
						printf("(key_values + key_value_index)->key=%p\n", (key_values + key_value_index)->key);
						printf("(key_values + key_value_index)->key=\"%s\"\n", (key_values + key_value_index)->key);
						*/
					}
				}
			}
		}
	}
	free(buffer);

	*(key_values_length) = key_values_found;

	return key_values;
}

#endif