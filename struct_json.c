/*
 * struct_json.c - Fast json parsing using structs and macros
 * Copyright (C) 2017 Christopher Chianelli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "struct_json.h"

void copy_json_remove_insignificant_whitespace(char *dest, const char *src)
{
	int in_string = 0;
	while (*src)
	{
		if (in_string)
		{
			in_string = (*src == '"')? 0 : 1;
			*dest = *src;
			dest++;
		}
		else
		{
			if (*src == '"')
			{
				in_string = 1;
				*dest = *src;
				dest++;
			}
			else if (*src == ' ' || *src == '\t' || *src == '\n')
			{
				//Do nothing
			}
			else
			{
				*dest = *src;
				dest++;
			}
		}
		src++;
	}
}

size_t get_json_list_length(const char *src)
{
	int in_string = 0;
	int brace_count = 0;
	size_t out = 0;

	if (src[1] == ']')
		return out;
	else
		out = 1;

	while (*src)
	{
		if (in_string)
		{
			in_string = (*src == '"')? 0 : 1;
		}
		else
		{
			if (*src == '"')
			{
				in_string = 1;
			}
			else if (*src == '{' || *src == '[')
			{
				brace_count++;
			}
			else if (*src == '}' || *src == ']')
			{
				brace_count--;
			}
			else if (*src == ',' && brace_count == 1)
			{
				out++;
			}
		}
		src++;
	}
	return out;
}

char *get_next_json_list_item_end(char *src)
{
	int in_string = 0;
	int brace_count = 0;

	while (*src)
	{
		if (in_string)
		{
			in_string = (*src == '"')? 0 : 1;
		}
		else
		{
			if (*src == '"')
			{
				in_string = 1;
			}
			else if (*src == '{' || *src == '[')
			{
				brace_count++;
			}
			else if (*src == '}' || *src == ']')
			{
				brace_count--;
			}
			else if (*src == ',' && brace_count == 0)
			{
				return src;
			}
		}
		src++;
	}
	return src - 1;
}

size_t __int_list_string_length(struct int_list list)
{
	size_t out = 2;
	char dummy[1];

	for (int i = 0; i < list.length; i++)
	{
		out += snprintf(dummy, sizeof(dummy), "%d", list.list[i]) + 1;
	}

	return out;
}

size_t __float_list_string_length(struct float_list list)
{
	size_t out = 2;
	char dummy[1];

	for (int i = 0; i < list.length; i++)
	{
		out += snprintf(dummy, sizeof(dummy), "%f", list.list[i]) + 1;
	}

	return out;
}

size_t __json_list_string_length(struct json_list list)
{
	size_t out = 2;

	for (int i = 0; i < list.length; i++)
	{
		out += strlen(list.list[i]) + 1;
	}

	return out;
}

size_t __string_list_string_length(struct string_list list)
{
	size_t out = 2;

	for (int i = 0; i < list.length; i++)
	{
		out += strlen(list.list[i]) + 3;
	}

	return out;
}
