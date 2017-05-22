#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "struct_json.h"

void fail_test(const char *msg)
{
	fprintf(stderr,"%s\n",msg);
	exit(1);
}

int main(int argc, char **argv)
{
	char *test_json = "{\"a\":true,"
			          "\"b\":1,"
			          "\"c\":2.500000,"
			          "\"d\":\"hello\","
			          "\"e\":{a:10,b:2},"
			          "\"f\":[true,false],"
				      "\"g\":[1,2],"
				      "\"h\":[2.500000,4.500000],"
					  "\"i\":[\"hello\",\"goodbye\"],"
				      "\"j\":[{a:10,b:2},{c:2,d:1}]}";
	struct {int a; int b; float c; char *d; char *e;
		   struct bool_list f; struct int_list g; struct float_list h;
	       struct string_list i; struct json_list j;} test_struct;

	DECODE_JSON_AS_STRUCT(test_json,typeof(test_struct),test_struct,
			a,bool,
			b,int,
			c,float,
			d,string,
			e,json,
			f,bool_list,
			g,int_list,
			h,float_list,
			i,string_list,
			j,json_list);

	if (test_struct.a != 1)
		fail_test("DECODE FAILED: BOOL does not have expected value");
	if (test_struct.b != 1)
		fail_test("DECODE FAILED: INT does not have expected value");
	if (fabsf(test_struct.c - 2.5f) > 0.01f)
		fail_test("DECODE FAILED: FLOAT does not have expected value");
	if (strcmp(test_struct.d,"hello"))
		fail_test("DECODE FAILED: STRING does not have expected value");
	if (strcmp(test_struct.e, "{a:10,b:2}"))
		fail_test("DECODE FAILED: JSON does not have expected value");
	if (test_struct.f.length != 2 || test_struct.f.list[0] != 1 || test_struct.f.list[1] != 0)
		fail_test("DECODE FAILED: BOOL LIST does not have expected value");
	if (test_struct.g.length != 2 || test_struct.g.list[0] != 1 || test_struct.g.list[1] != 2)
		fail_test("DECODE FAILED: INT LIST does not have expected value");
	if (test_struct.h.length != 2 || fabsf(test_struct.h.list[0] - 2.5f) > 0.01f || fabsf(test_struct.h.list[1] - 4.5f) > 0.01f)
		fail_test("DECODE FAILED: FLOAT LIST does not have expected value");
	if (test_struct.i.length != 2 || strcmp(test_struct.i.list[0],"hello") || strcmp(test_struct.i.list[1],"goodbye"))
		fail_test("DECODE FAILED: STRING LIST does not have expected value");
	if (test_struct.j.length != 2 || strcmp(test_struct.j.list[0],"{a:10,b:2}") || strcmp(test_struct.j.list[1],"{c:2,d:1}"))
		fail_test("DECODE FAILED: JSON LIST does not have expected value");

	printf("DECODE PASSED\n");

	char *out;
	ENCODE_STRUCT_AS_JSON(test_struct,typeof(test_struct),out,
			a,bool,
			b,int,
			c,float,
			d,string,
			e,json,
			f,bool_list,
			g,int_list,
			h,float_list,
			i,string_list,
			j,json_list);

	if (strcmp(test_json,out))
	{
		fail_test("ENCODE FAILED: JSON does not match expected output");
	}
	else
	{
		printf("ENCODE PASSED\n");
	}

	return 0;
}
