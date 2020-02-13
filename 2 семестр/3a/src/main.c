// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _MSC_VER
#error
#error	MSVC compiler needed
#error
#endif /* !_MSC_VER */

#include "Tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* proceed(char* str);

signed main(void)
{
	char* str = NULL;
	while (1)
	{
		printf("Enter string\n> ");
		str = get_str();
		if (!str)
			break;
		
		str = proceed(str);

		printf("\nResult\n%s\n\n", str);

		free(str);
	}

	return 0;
}

char* proceed(char* str)
{
	size_t pos = 0;
	size_t len = strlen(str);
	
	char first = str[0];
	char last  = str[len - 1];

	char* res = get_pattern(first, last, &pos, str);
	size_t res_len = strlen(res);
	while (pos < len)
	{
		char* pattern = get_pattern(first, last, &pos, str);
		if (pattern)
		{
			res_len += 1 + strlen(pattern);
			char* tmp_ptr = (char*)realloc(res, res_len + 1);
			if (!tmp_ptr)
			{
				printf("Cannot reallocate memory for new pattern\n");

				free(res);

				return str;
			}
			
			res = tmp_ptr;

			strcat_s(res, res_len + 1, " ");
			strcat_s(res, res_len + 1, pattern);

			free(pattern);
		}
		else
			break;
	}

	free(str);

	return res;
}
