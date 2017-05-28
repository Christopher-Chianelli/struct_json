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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map-macro/map.h"

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

//Public methods
extern size_t get_json_list_length(const char *src);
extern char *get_next_json_list_item_end(char *src);
extern char *http_request(const char *host, int portno, const char *method, const char *path, const char *data, const char **headers, size_t header_count);
extern char *ajax_get_request(const char *url, const char *params);
extern char *ajax_post_request(const char *url, const char *data);
extern char *ajax_put_request(const char *url, const char *data);
extern char *ajax_delete_request(const char *url, const char *data);
extern char *json_string_to_c_string(char *str);
extern char *c_string_to_json_string(char *str);

//Private methods
extern void __struct_json___copy_json_remove_insignificant_whitespace(char *dest, const char *src);
extern size_t __struct_json___int_list_string_length(struct int_list list);
extern size_t __struct_json___float_list_string_length(struct float_list list);
extern size_t __struct_json___json_list_string_length(struct json_list list);
extern size_t __struct_json___string_list_string_length(struct string_list list);
extern int __struct_json___required_chars_to_escape(char *str);
extern char __struct_json___parse_hex_digits(char *digits);
extern char __struct_json___parse_hex_digit(char digit);

#define MAP0_2ARG(f, x, y, peek, ...) f(x,y) MAP_NEXT(peek, MAP1_2ARG)(f, peek, __VA_ARGS__)
#define MAP1_2ARG(f, x, y, peek, ...) f(x,y) MAP_NEXT(peek, MAP0_2ARG)(f, peek, __VA_ARGS__)
#define MAP_2ARG(f, ...) EVAL(MAP1_2ARG(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#define __struct_json___int_ATTRIBUTE(name) int name;
#define __struct_json___float_ATTRIBUTE(name) float name;
#define __struct_json___bool_ATTRIBUTE(name) unsigned char name;
#define __struct_json___string_ATTRIBUTE(name) char *name;
#define __struct_json___json_ATTRIBUTE(name) char *name;

#define __struct_json___int_list_ATTRIBUTE(name) struct int_list name;
#define __struct_json___float_list_ATTRIBUTE(name) struct float_list name;
#define __struct_json___bool_list_ATTRIBUTE(name) struct bool_list name;
#define __struct_json___string_list_ATTRIBUTE(name) struct string_list name;
#define __struct_json___json_list_ATTRIBUTE(name) struct json_list name;

#define __struct_json___LIST_ATTRIBUTE(name,type) __struct_json___##type##_ATTRIBUTE(name)

#define __struct_json___DECODE_string_ATTRIBUTE(attr,value) {\
	__out->attr = json_string_to_c_string(value);\
}

#define __struct_json___DECODE_json_ATTRIBUTE(attr,value) {\
	char *__temp = malloc(strlen(value)+1);\
	strcpy(__temp,value);\
	__out->attr = __temp;\
}

#define __struct_json___DECODE_int_ATTRIBUTE(attr,value) {\
	__out->attr = atoi(value);\
}

#define __struct_json___DECODE_bool_ATTRIBUTE(attr,value) {\
	__out->attr = (!strcmp(value,"true"))? 1 : 0;\
}

#define __struct_json___DECODE_float_ATTRIBUTE(attr,value) {\
	__out->attr = atof(value);\
}

#define __struct_json___DECODE_string_list_ATTRIBUTE(attr,value) {\
	__out->attr.length = get_json_list_length(value);\
	value = value + 1;\
	__out->attr.list = malloc(sizeof(char *)*__out->attr.length);\
	for (int __i = 0; __i < __out->attr.length; __i++){\
		char *__item_end = get_next_json_list_item_end(value);\
		*__item_end = '\0';\
		__out->attr.list[__i] = json_string_to_c_string(value);\
		value = __item_end + 1;\
	}\
}

#define __struct_json___DECODE_json_list_ATTRIBUTE(attr,value) {\
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

#define __struct_json___DECODE_int_list_ATTRIBUTE(attr,value) {\
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

#define __struct_json___DECODE_bool_list_ATTRIBUTE(attr,value) {\
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

#define __struct_json___DECODE_float_list_ATTRIBUTE(attr,value) {\
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

#define __struct_json___int_STRING_LENGTH_OF_ATTRIBUTE(attr) {\
	__length += snprintf(__dummy, sizeof(__dummy), "%d", __src->attr);\
}

#define __struct_json___bool_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += 5;

#define __struct_json___float_STRING_LENGTH_OF_ATTRIBUTE(attr) {\
		__length += snprintf(__dummy, sizeof(__dummy), "%f", __src->attr);\
}

#define __struct_json___string_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __struct_json___required_chars_to_escape(__src->attr) + 2;

#define __struct_json___json_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += strlen(__src->attr);

#define __struct_json___int_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __struct_json___int_list_string_length(__src->attr);
#define __struct_json___bool_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += 2 + 6 * __src->attr.length;
#define __struct_json___float_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __struct_json___float_list_string_length(__src->attr);
#define __struct_json___string_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __struct_json___string_list_string_length(__src->attr);
#define __struct_json___json_list_STRING_LENGTH_OF_ATTRIBUTE(attr) __length += __struct_json___json_list_string_length(__src->attr);

#define __struct_json___PUT_int_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":%d,", #attr, __src->attr);\
}

#define __struct_json___PUT_float_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":%f,", #attr, __src->attr);\
}

#define __struct_json___PUT_bool_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":%s,", #attr, (__src->attr)? "true" : "false");\
}

#define __struct_json___PUT_string_INTO_STRING(attr) {\
	char *__temp = c_string_to_json_string(__src->attr);\
	__buf += sprintf(__buf,"\"%s\":\"%s\",", #attr, __temp);\
	free(__temp);\
}

#define __struct_json___PUT_json_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":%s,", #attr, __src->attr);\
}

#define __struct_json___PUT_int_list_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":",#attr);\
	if (__src->attr.length == 0){\
		__buf += sprintf(__buf,"[],");\
	}\
	else{\
		__buf += sprintf(__buf,"[");\
		int __i = 0;\
		for (; __i < __src->attr.length - 1; __i++){\
			__buf += sprintf(__buf,"%d,",__src->attr.list[__i]);\
		}\
		__buf += sprintf(__buf,"%d],",__src->attr.list[__i]);\
    }\
}

#define __struct_json___PUT_float_list_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":",#attr);\
	if (__src->attr.length == 0){\
		__buf += sprintf(__buf,"[],");\
	}\
	else{\
		__buf += sprintf(__buf,"[");\
		int __i = 0;\
		for (; __i < __src->attr.length - 1; __i++){\
			__buf += sprintf(__buf,"%f,",__src->attr.list[__i]);\
		}\
		__buf += sprintf(__buf,"%f],",__src->attr.list[__i]);\
	}\
}

#define __struct_json___PUT_bool_list_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":",#attr);\
	if (__src->attr.length == 0){\
		__buf += sprintf(__buf,"[],");\
	}\
	else{\
		__buf += sprintf(__buf,"[");\
		int __i = 0;\
		for (;__i < __src->attr.length - 1; __i++){\
			__buf += sprintf(__buf,"%s,", (__src->attr.list[__i])? "true" : "false");\
		}\
		__buf += sprintf(__buf,"%s],",(__src->attr.list[__i])? "true" : "false");\
	}\
}

#define __struct_json___PUT_string_list_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":",#attr);\
	if (__src->attr.length == 0){\
		__buf += sprintf(__buf,"[],");\
	}\
	else{\
		__buf += sprintf(__buf,"[");\
		int __i = 0;\
		char *__temp;\
		for (; __i < __src->attr.length - 1; __i++){\
			__temp = c_string_to_json_string(__src->attr.list[__i]);\
			__buf += sprintf(__buf,"\"%s\",", __temp);\
			free(__temp);\
		}\
		__temp = c_string_to_json_string(__src->attr.list[__i]);\
		__buf += sprintf(__buf,"\"%s\"],", __temp);\
		free(__temp);\
	}\
}

#define __struct_json___PUT_json_list_INTO_STRING(attr) {\
	__buf += sprintf(__buf,"\"%s\":",#attr);\
	if (__src->attr.length == 0){\
		__buf += sprintf(__buf,"[],");\
	}\
	else{\
		__buf += sprintf(__buf,"[");\
		int __i = 0;\
		for (; __i < __src->attr.length - 1; __i++){\
			__buf += sprintf(__buf,"%s,", __src->attr.list[__i]);\
		}\
		__buf += sprintf(__buf,"%s],", __src->attr.list[__i]);\
	}\
}

#define __struct_json___GET_STRING_LENGTH_OF_SINGLE_ATTRIBUTE_OF_STRUCT(attr,type) {\
	 __length += strlen(#attr) + 3;\
	 __struct_json___##type##_STRING_LENGTH_OF_ATTRIBUTE(attr);\
	 __length++;\
}

#define __struct_json___PUT_SINGLE_ATTRIBUTE_OF_STRUCT(attr,type) {\
	__struct_json___PUT_##type##_INTO_STRING(attr);\
}

#define __struct_json___SINGLE_ATTRIBUTE_OF_JSON(attr,type) \
	else if (!strcmp(#attr,__name)) {\
		__struct_json___DECODE_##type##_ATTRIBUTE(attr,__value)\
	}

#define FOREACH_ITEM_IN_JSON(src,key,value)\
	for (int __ready = 0,__brace_count = 0,\
			 __state = 0,__in_string = 0,\
			 __not_done = 1;\
			 __not_done;\
			 __not_done=0)\
	for (char *__source = (src),*__json = malloc(strlen(__source)),\
			*__to_free = __json;\
	        (__not_done)?(__struct_json___copy_json_remove_insignificant_whitespace(__json,__source),0)\
	                  :(free(__to_free),1),__not_done;__not_done=0)\
	for (char *key,*value;\
	        *__json || __ready;\
			__ready=0,(\
	        (__in_string)?\
			    (__in_string = (*__json == '"')? 0 : 1):\
			((*__json == '"')?\
					((__state == 0)?\
							(__state = 1,key = __json + 1,0):\
					 (__state == 1)?\
							(__state = 2, *__json = '\0',0):\
					 ("ELSE",\
					         __in_string = 1)\
			):\
            (*__json == ':' && __state == 2)?\
            		(__state = 3,value = __json + 1,0):\
			(*__json == '{' || *__json == '[')?\
					(__brace_count++):\
			(*__json == '}' || *__json == ']')?\
					(__brace_count--,(!__brace_count)?(*__json='\0',__state=0,__ready=1):0):\
			(*__json == ',' && __brace_count == 1)?\
					(*__json='\0',__state=0,__ready=1):\
					0)\
			),__json++)\
			if (__ready)

#define UPDATE_STRUCT_FROM_JSON(src,type,output,...) {\
	type *__out = &(output);\
	FOREACH_ITEM_IN_JSON(src,__name,__value){\
		if (0){}\
			MAP_2ARG(__struct_json___SINGLE_ATTRIBUTE_OF_JSON, __VA_ARGS__)\
    }\
}

#define CREATE_STRUCT_FROM_JSON(src,output,...) \
	struct{MAP_2ARG(__struct_json___LIST_ATTRIBUTE,__VA_ARGS__)} output;\
	{\
	struct{MAP_2ARG(__struct_json___LIST_ATTRIBUTE,__VA_ARGS__)} *__out = (void *)&(output);\
	FOREACH_ITEM_IN_JSON(src,__name,__value){\
		if (0){}\
			MAP_2ARG(__struct_json___SINGLE_ATTRIBUTE_OF_JSON, __VA_ARGS__)\
    }\
}

#define GET_ATTRIBUTE_FROM_JSON(src,output,attr,json_type) {\
	struct {__struct_json___LIST_ATTRIBUTE(attr,json_type)} *__out = (void *)&(output);\
	FOREACH_ITEM_IN_JSON(src,__name,__value){\
			if (0){}\
			__struct_json___SINGLE_ATTRIBUTE_OF_JSON(attr,json_type)\
	}\
}

#define CREATE_JSON_FROM_STRUCT(source,type,dest,...) {\
	char *__dst;\
	char *__buf;\
	type *__src = &(source);\
	char __dummy[1];\
	size_t __length = 2;\
	MAP_2ARG(__struct_json___GET_STRING_LENGTH_OF_SINGLE_ATTRIBUTE_OF_STRUCT, __VA_ARGS__)\
	__dst = malloc(__length);\
	__buf = __dst + 1;\
	__dst[0] = '{';\
	__dst[1] = '\0';\
	MAP_2ARG(__struct_json___PUT_SINGLE_ATTRIBUTE_OF_STRUCT, __VA_ARGS__)\
	*(__buf-1) = '}';\
	(dest) = __dst;\
}

#endif
