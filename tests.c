#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "struct_json.h"

void fail_test(const char *msg)
{
	fprintf(stderr,"%s\n",msg);
}

void test_json_bool()
{
	char *test_json1 = "{\"a\":true}";
	char *test_json2 = "{\"a\":false}";

	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,bool);
	CREATE_STRUCT_FROM_JSON(test_json2,out2,a,bool);
	if (out1.a != 1 || out2.a != 0)
		fail_test("Boolean decode failed");
	else
		printf("Boolean decode passed\n");
}

void test_json_int()
{
	char *test_json1 = "{\"a\":42}";

	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,int);
	if (out1.a != 42)
		fail_test("Int decode failed");
	else
		printf("Int decode passed\n");
}

void test_json_float()
{
	char *test_json1 = "{\"a\":1.5}";

	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,float);
	if (out1.a < 1.49 || out1.a > 1.51)
		fail_test("Float decode failed");
	else
		printf("Float decode passed\n");
}

void test_json_string()
{
	char *test_json1 = "{\"a\":\"Hello World!\\n\"}";

	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,string);
	if (strcmp(out1.a,"Hello World!\n"))
		fail_test("String decode failed");
	else
		printf("String decode passed\n");
}

void test_json_json()
{
	char *test_json1 = "{\"a\":{\"a\":1,\"b\":2}}";
	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,json);
	int a,b;
	GET_ATTRIBUTE_FROM_JSON(out1.a,a,a,int);
	GET_ATTRIBUTE_FROM_JSON(out1.a,b,b,int);
	if (a != 1 || b != 2)
		fail_test("JSON decode failed");
	else
		printf("JSON decode passed\n");
}

void test_json_bool_list()
{
	char *test_json1 = "{\"a\":[true,false]}";

	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,bool_list);
	if (out1.a.length != 2 || out1.a.list[0] != 1 || out1.a.list[1] != 0)
		fail_test("Boolean list decode failed");
	else
		printf("Boolean list decode passed\n");
}

void test_json_int_list()
{
	char *test_json1 = "{\"a\":[1,2]}";

	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,int_list);
	if (out1.a.length != 2 || out1.a.list[0] != 1 || out1.a.list[1] != 2)
		fail_test("Int list decode failed");
	else
		printf("Int list decode passed\n");
}

void test_json_float_list()
{
	char *test_json1 = "{\"a\":[1.5,2.5]}";

	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,float_list);
	if (out1.a.length != 2 || fabsf(out1.a.list[0]-1.5f) > 0.1f || fabsf(out1.a.list[1]-2.5f) > 0.1f)
		fail_test("Float list decode failed");
	else
		printf("Float list decode passed\n");
}

void test_json_string_list()
{
	char *test_json1 = "{\"a\":[\"hi\",\"bye\"]}";

	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,string_list);
	if (out1.a.length != 2 || strcmp(out1.a.list[0],"hi") || strcmp(out1.a.list[1],"bye"))
		fail_test("String list decode failed");
	else
		printf("String list decode passed\n");
}

void test_json_json_list()
{
	char *test_json1 = "{\"a\":[{\"a\":1},{\"b\":2}]}";
	CREATE_STRUCT_FROM_JSON(test_json1,out1,a,json_list);
	if (out1.a.length != 2)
	{
		fail_test("JSON list decode failed");
		return;
	}
	int a,b;
	GET_ATTRIBUTE_FROM_JSON(out1.a.list[0],a,a,int);
	GET_ATTRIBUTE_FROM_JSON(out1.a.list[1],b,b,int);
	if (a != 1 || b != 2)
		fail_test("JSON list decode failed");
	else
		printf("JSON list decode passed\n");
}

int main(int argc, char **argv)
{
	test_json_bool();
	test_json_int();
	test_json_float();
	test_json_string();
	test_json_json();

	test_json_bool_list();
	test_json_int_list();
	test_json_float_list();
	test_json_string_list();
	test_json_json_list();
	return 0;
}
