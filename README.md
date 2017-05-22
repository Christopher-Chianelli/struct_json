# struct_json
Fast JSON parsing in C using structs and Macros

## What is this?
struct_json is a C libary that provide Macros for fast JSON parsing using structs.
Usually, we know what attributes a JSON has and their types when we make an AJAX
request. struct_json take advantage of this by letting the user specify the JSON
attributes and datatypes so it can generate a dynamic function that parses the JSON
and put the data into an already existing struct.

## Call format
DECODE_JSON_AS_STRUCT(json,typeof(struct),out,(attribute,type)...)
ENCODE_STRUCT_AS_JSON(struct,typeof(struct),out,(attribute,type)...)

## Supported Types
- int
- float
- string
- json (stored as a string)
- bool (stored as an unsigned char)
- type_list (any of the above types, but as a list)

## Example Usage
### JSON to Struct, containing attributes status (an int) and msg (a string)
```
char *json = "{\"status\":200,\"msg\":\"OK\"}";
struct {int status; char *msg;} out;
DECODE_JSON_AS_STRUCT(json,typeof(out),out,status,int,msg,string)
```

### Struct to JSON, containing attributes status (an int) and msg (a string)
```
char *json;
struct {int status; char *msg;} out;
ENCODE_STRUCT_AS_JSON(out,typeof(out),json,status,int,msg,string)
```
