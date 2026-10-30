#ifndef __GHDB_H
#define __GHDB_H

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
/*
struct RECORD
{
	char plant_name[PLANT_NAME_LENGTH + 1];
	char latin_name[LATIN_NAME_LENGTH + 1];
	char height[HEIGHT_LENGTH + 1];
	char width[WIDTH_LENGTH + 1];
	char planting_depth[PLANTING_DEPTH_LENGTH + 1];
	char seed_size[SEED_SIZE_LENGTH + 1];
	char seed_need_light[SEED_NEED_LIGHT_LENGTH + 1];
	char seed_scarification[SEED_SCARIFICATION_LENGTH + 1];
	char ph[PH_LENGTH + 1];
	char ec[EC_LENGTH + 1];
	char day_light_interval[DAY_LIGHT_INTERVAL_LENGTH + 1];
	char photoperiod_hours[PHOTOPERIOD_HOURS_LENGTH + 1];
	char light_lower[LIGHT_LOWER_LENGTH + 1];
	char light_optimal[LIGHT_OPTIMAL_LENGTH + 1];
	char light_upper[LIGHT_UPPER_LENGTH + 1];
	char nitrogen_lower[NITROGEN_LOWER_LENGTH + 1];
	char nitrogen_optimal[NITROGEN_OPTIMAL_LENGTH + 1];
	char nitrogen_upper[NITROGEN_UPPER_LENGTH + 1];
	char phosphorus_lower[PHOSPHORUS_LOWER_LENGTH + 1];
	char phosphorus_optimal[PHOSPHORUS_OPTIMAL_LENGTH + 1];
	char phosphorus_upper[PHOSPHORUS_UPPER_LENGTH + 1];
	char potassium_lower[POTASSIUM_LOWER_LENGTH + 1];
	char potassium_optimal[POTASSIUM_OPTIMAL_LENGTH + 1];
	char potassium_upper[POTASSIUM_UPPER_LENGTH + 1];
	char germination_lower[GERMINATION_LOWER_LENGTH + 1];
	char germination_normal[GERMINATION_NORMAL_LENGTH + 1];
	char germination_upper[GERMINATION_UPPER_LENGTH + 1];
	char transplanting_lower[TRANSPLANTING_LOWER_LENGTH + 1];
	char transplanting_optimal[TRANSPLANTING_OPTIMAL_LENGTH + 1];
	char transplanting_upper[TRANSPLANTING_UPPER_LENGTH + 1];
	char maturity_lower[MATURITY_LOWER_LENGTH + 1];
	char maturity_optimal[MATURITY_OPTIMAL_LENGTH + 1];
	char maturity_upper[MATURITY_UPPER_LENGTH + 1];
	char frost_tolerance[FROST_TOLERANCE_LENGTH + 1];
	char flowering[FLOWERING_LENGTH + 1];
	char pollination_primary[POLLINATION_PRIMARY_LENGTH + 1];
	char pollination_secondary[POLLINATION_SECONDARY_LENGTH + 1];
};
*/
char * html_encode(char *string);
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
						result_length +- 2;
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

int field_index(char *name)
{
	//printf("name=\"%s\"\n", name);
	for (int i = 0; i < NUM_FIELDS; i++)
	{
		//printf("fields[i].n=\"%s\"\n", fields[i].n);
		if (strcmp(fields[i].n, name) == 0)
		{
			return i;
		}
	}
	return -1;
}

#endif