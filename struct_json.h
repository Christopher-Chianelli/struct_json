/*
 * struct_json.h - Fast json parsing using structs and macros
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

#ifndef STRUCT_JSON_H
#define STRUCT_JSON_H

#include <stdlib.h>
#include <string.h>
#include "map.h"

struct int_list
{
	size_t length;
	int *list;
};

struct bool_list
{
	size_t length;
	unsigned char *list;
};

struct float_list
{
	size_t length;
	float *list;
};

struct string_list
{
	size_t length;
	char **list;
};

struct json_list
{
	size_t length;
	char **list;
};

extern void copy_json_remove_insignificant_whitespace(char *dest, const char *src);
extern size_t get_json_list_length(const char *src);
extern char *get_next_json_list_item_end(char *src);

extern size_t __int_list_string_length(struct int_list list);
extern size_t __float_list_string_length(struct float_list list);
extern size_t __json_list_string_length(struct json_list list);
extern size_t __string_list_string_length(struct string_list list);

#define MAP0_2ARG(f, x, y, peek, ...) f(x,y) MAP_NEXT(peek, MAP1_2ARG)(f, peek, __VA_ARGS__)
#define MAP1_2ARG(f, x, y, peek, ...) f(x,y) MAP_NEXT(peek, MAP0_2ARG)(f, peek, __VA_ARGS__)
#define MAP_2ARG(f, ...) EVAL(MAP1_2ARG(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#define __DECODE_string_ATTRIBUTE(attr,value) {\
	char *__temp = malloc(strlen(value));\
	strcpy(__temp,value + 1);\
	__temp[strlen(value)-2]='\0';\
	__out->attr = __temp;\
}

#define __DECODE_json_ATTRIBUTE(attr,value) {\
	char *__temp = malloc(strlen(value)+1);\
	strcpy(__temp,value);\
	__out->attr = __temp;\
}

#define __DECODE_int_ATTRIBUTE(attr,value) {\
	__out->attr = atoi(value);\
}

#define __DECODE_bool_ATTRIBUTE(attr,value) {\
	__out->attr = (!strcmp(value,"true"))? 1 : 0;\
}

#define __DECODE_float_ATTRIBUTE(attr,value) {\
	__out->attr = atof(value);\
}

#define __DECODE_string_list_ATTRIBUTE(attr,value) {\
	__out->attr.length = get_json_list_length(value);\
	value = value + 1;\
	__out->attr.list = malloc(sizeof(char *)*__out->attr.length);\
	for (int __i = 0; __i < __out->attr.length; __i++){\
		char *__item_end = get_next_json_list_item_end(value);\
		*__item_end = '\0';\
		char *__temp = malloc(strlen(value));\
		strcpy(__temp,value + 1);\
		__temp[strlen(value)-2]='\0';\
		__out->attr.list[__i] = __temp;\
		value = __item_end + 1;\
	}\
}

#define __DECODE_json_list_ATTRIBUTE(attr,value) {\
	__out->attr.length = get_json_list_length(value);\
	value = value + 1;\
	__out->attr.list = malloc(sizeof(char *)*__out->attr.length);\
	for (int __i = 0; __i < __out->attr.length; __i++){\
		char *__item_end = get_next_json_list_item_end(value);\
		*__item_end = '\0';\
		char *__temp = malloc(strlen(value)+1);\
		strcpy(__temp,value);\
		__out->attr.list[__i] = __temp;\
		value = __item_end + 1;\
	}\
}

#define __DECODE_int_list_ATTRIBUTE(attr,value) {\
	__out->attr.length = get_json_list_length(value);\
	value = value + 1;\
	__out->attr.list = malloc(sizeof(char *)*__out->attr.length);\
	for (int __i = 0; __i < __out->attr.length; __i++){\
		char *__item_end = get_next_json_list_item_end(value);\
		*__item_end = '\0';\
		__out->attr.list[__i] = atoi(value);\
		value = __item_end + 1;\
	}\
}

#define __DECODE_bool_list_ATTRIBUTE(attr,value) {\
	__out->attr.length = get_json_list_length(value);\
	value = value + 1;\
	__out->attr.list = malloc(sizeof(char *)*__out->attr.length);\
	for (int __i = 0; __i < __out->attr.length; __i++){\
		char *__item_end = get_next_json_list_item_end(value);\
		*__item_end = '\0';\
		__out->attr.list[__i] = (!strcmp(value,"true"))? 1 : 0;\
		value = __item_end + 1;\
	}\
}

#define __DECODE_float_list_ATTRIBUTE(attr,value) {\
	__out->attr.length = get_json_list_length(value);\
	value = value + 1;\
	__out->attr.list = malloc(sizeof(char *)*__out->attr.length);\
	for (int __i = 0; __i < __out->attr.length; __i++){\
		char *__item_end = get_next_json_list_item_end(value);\
		*__item_end = '\0';\
		__out->attr.list[__i] = atof(value);\
		value = __item_end + 1;\
	}\
}

#define __int_STRING_LENGTH_OF_ATTRIBUTE(attr) {\
	__length += snprintf(__dummy, sizeof(__dummy), "%d", __src->attr);\
}

#define __bool_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += 5;

#define __float_STRING_LENGTH_OF_ATTRIBUTE(attr) {\
		__length += snprintf(__dummy, sizeof(__dummy), "%f", __src->attr);\
}

#define __string_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += strlen(__src->attr) + 2;

#define __json_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += strlen(__src->attr);

#define __int_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __int_list_string_length(__src->attr);
#define __bool_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += 2 + 6 * __src->length;
#define __float_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __float_list_string_length(__src->attr);
#define __string_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __json_list_string_length(__src->attr);
#define __json_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __string_list_string_length(__src->attr);

#define __PUT_int_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":%d,", __dst, #attr, __src->attr);\
}

#define __PUT_float_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":%f,", __dst, #attr, __src->attr);\
}

#define __PUT_bool_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":%s,", __dst, #attr, (__src->attr)? "true" : "false");\
}

#define __PUT_string_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":\"%s\",", __dst, #attr, __src->attr);\
}

#define __PUT_json_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":%s,", __dst, #attr, __src->attr);\
}
//------------------------------------------------------------
#define __PUT_int_list_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":",__dst,#attr);\
	if (__src->attr.length == 0){\
		sprintf(__dst,"%s[],",__dst);\
	}\
	else{\
		sprintf(__dst,"%s[",__dst);\
		int __i = 0;\
		for (; __i < __src->attr.length - 1; __i++){\
			sprintf(__dst,"%s%d,",__dst,__src->attr.list[__i]);\
		}\
		sprintf(__dst,"%s%d],",__dst,__src->attr.list[__i]);\
    }\
}

#define __PUT_float_list_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":",__dst,#attr);\
	if (__src->attr.length == 0){\
		sprintf(__dst,"%s[],",__dst);\
	}\
	else{\
		sprintf(__dst,"%s[",__dst);\
		int __i = 0;\
		for (; __i < __src->attr.length - 1; __i++){\
			sprintf(__dst,"%s%f,",__dst,__src->attr.list[__i]);\
		}\
		sprintf(__dst,"%s%f],",__dst,__src->attr.list[__i]);\
	}\
}

#define __PUT_bool_list_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":",__dst,#attr);\
	if (__src->attr.length == 0){\
		sprintf(__dst,"%s[],",__dst);\
	}\
	else{\
		sprintf(__dst,"%s[",__dst);\
		int __i = 0;\
		for (;__i < __src->attr.length - 1; __i++){\
			sprintf(__dst,"%s%s,",__dst, (__src->attr.list[__i])? "true" : "false");\
		}\
		sprintf(__dst,"%s%s],",__dst,(__src->attr.list[__i])? "true" : "false");\
	}\
}

#define __PUT_string_list_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":",__dst,#attr);\
	if (__src->attr.length == 0){\
		sprintf(__dst,"%s[],",__dst);\
	}\
	else{\
		sprintf(__dst,"%s[",__dst);\
		int __i = 0;\
		for (; __i < __src->attr.length - 1; __i++){\
			sprintf(__dst,"%s\"%s\",",__dst, __src->attr.list[__i]);\
		}\
		sprintf(__dst,"%s\"%s\"],",__dst, __src->attr.list[__i]);\
	}\
}

#define __PUT_json_list_INTO_STRING(attr) {\
	sprintf(__dst,"%s\"%s\":",__dst,#attr);\
	if (__src->attr.length == 0){\
		sprintf(__dst,"%s[],",__dst);\
	}\
	else{\
		sprintf(__dst,"%s[",__dst);\
		int __i = 0;\
		for (; __i < __src->attr.length - 1; __i++){\
			sprintf(__dst,"%s%s,",__dst, __src->attr.list[__i]);\
		}\
		sprintf(__dst,"%s%s],",__dst, __src->attr.list[__i]);\
	}\
}
//----------------------------------------------------------------

#define __GET_STRING_LENGTH_OF_SINGLE_ATTRIBUTE_OF_STRUCT(attr,type) {\
	 __length += strlen(#attr) + 3;\
	 __##type##_STRING_LENGTH_OF_ATTRIBUTE(attr);\
	 __length++;\
}

#define __PUT_SINGLE_ATTRIBUTE_OF_STRUCT(attr,type) {\
	__PUT_##type##_INTO_STRING(attr);\
}

#define __SINGLE_ATTRIBUTE_OF_JSON(attr,type) {\
	if (!strcmp(#attr,__name)) {\
		__DECODE_##type##_ATTRIBUTE(attr,__value)\
	}\
}

#define DECODE_JSON_AS_STRUCT(src,type,output,...) {\
	char *__source = (src);\
	char *__json = malloc(strlen(__source));\
	copy_json_remove_insignificant_whitespace(__json,__source);\
    char *__to_free = __json;\
	type *__out = &(output);\
	char *__name;\
	char *__value;\
	int __in_string = 0;\
	int __brace_count = 0;\
	int __state = 0;\
	\
	while (*__json) {\
		if (__in_string) {\
			__in_string = (*__json == '"')? 0 : 1;\
		}\
		else {\
			if (*__json == '"') {\
				if (__state == 0) {\
				__state = 1;\
				__name = __json + 1;\
				}\
				else if (__state == 1) {\
					__state = 2;\
					*__json = '\0';\
				}\
				else {\
					__in_string = 1;\
				}\
			}\
			else if (*__json == ':' && __state == 2){\
					__state = 3;\
					__value = __json + 1;\
			}\
			else if (*__json == '{' || *__json == '[') {\
				__brace_count++;\
			}\
			else if (*__json == '}' || *__json == ']') {\
				__brace_count--;\
				if (__brace_count == 0){\
					*__json = '\0';\
					MAP_2ARG(__SINGLE_ATTRIBUTE_OF_JSON, __VA_ARGS__)\
					__state = 0;\
				}\
			}\
			else if (*__json == ',' && __brace_count == 1) {\
				*__json = '\0';\
				MAP_2ARG(__SINGLE_ATTRIBUTE_OF_JSON, __VA_ARGS__)\
				__state = 0;\
			}\
		}\
		__json++;\
	}\
	free(__to_free);\
}

#define ENCODE_STRUCT_AS_JSON(source,type,dest,...) {\
	char *__dst;\
	type *__src = &(source);\
	char __dummy[1];\
	size_t __length = 2;\
	MAP_2ARG(__GET_STRING_LENGTH_OF_SINGLE_ATTRIBUTE_OF_STRUCT, __VA_ARGS__)\
	__dst = malloc(__length);\
	__dst[0] = '{';\
	__dst[1] = '\0';\
	MAP_2ARG(__PUT_SINGLE_ATTRIBUTE_OF_STRUCT, __VA_ARGS__)\
	__dst[strlen(__dst) - 1] = '}';\
	(dest) = __dst;\
}

#endif
