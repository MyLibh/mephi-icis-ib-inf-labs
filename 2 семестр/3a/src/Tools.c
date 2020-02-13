// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Tools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_LENGTH 128

char* get_str(void)
{
	char buf[BUFFER_LENGTH] = "";

	size_t str_len = 0;
	char*  str	   = (char*)malloc(1);
	if (!str)
	{
		printf("Cannot allocate memory for str\n");

		return NULL;
	}

	*str = '\0';
	
	int scanf_ret_val = 0;
	do
	{
		scanf_ret_val = scanf_s("%127[^\n]", buf, BUFFER_LENGTH);
		if (scanf_ret_val == 1)
		{
			str_len += strlen(buf);

			char* tmp_ptr = (char*)realloc(str, str_len + 1);
			if (!tmp_ptr)
			{
				printf("Cannot reallocate memory for str\n");

				free(str);

				return NULL;
			}

			str = tmp_ptr;
			strcat(str, buf);
		}
		else if (!scanf_ret_val)
		{
			scanf_s("%*c");
		}
		else
		{
			free(str);

			return NULL;
		}
	} while (scanf_ret_val > 0);
		
	return str;
}

char* remove_spaces(char* str) 
{
	char* res = (char*)malloc(strlen(str) + 1);
	if (!res)
	{
		printf("Cannot allocate memory for new str\n");

		exit(1);
	}

	size_t count = 0;
	for (size_t i = 0; str[i]; ++i)
	{
		if (!isspace(str[i]))
			res[count++] = str[i];
	}

	res[count] = '\0';

	free(str);
	str = NULL;

	char* ptr = (char*)realloc(res, count + 1);
	if (!ptr)
	{
		printf("Cannot reallocate space for new string\n");

		free(res);
	}

	return ptr;
}

char* get_pattern(const char first, const char last, size_t* position, const char* str)
{
	char* start = strchr(str + *position, first);
	if (!start)
		return NULL;

	char* end = strchr(start, last);

	*position = end - str + 1;

	size_t len = end - start;
	char* res = (char*)malloc(len + 1);
	if (!res)
	{
		printf("Cannot allocate space for pattern\n");

		exit(1);
	}

	strncpy_s(res, len + 1, ++start, len);

	res[!len ? 0 : len - 1] = '\0';

	return remove_spaces(res);
}