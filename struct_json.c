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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "struct_json.h"

int __struct_json___required_chars_to_escape(char *str)
{
	char *temp = str;
	int required_chars = 1;
	while (*temp)
	{
		if (*temp == '"' || *temp == '\\')
			required_chars++;
		required_chars++;
		temp++;
	}

	return required_chars;
}

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
			else if (*src == ' ' || *src == '\t' || *src == '\n' || *src == '\r')
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

char *escape_double_quotes_and_backslashes(char *str)
{
	char *temp = str;
	int required_chars = __struct_json___required_chars_to_escape(str);
	char *out = malloc(required_chars);
	int i = 0;

	while (*temp)
	{
		if (*temp == '"' || *temp == '\\')
		{
			out[i] = '\\';
			i++;
		}
		out[i] = *temp;
		i++;
		temp++;
	}
	out[i] = *temp;

	return out;
}

char *unescape_double_quotes_and_backslashes(char *str)
{
	char *temp = str + 1;
	int required_chars = 0;
	while (*temp)
	{
		if (*temp == '\\')
			temp++;
		required_chars++;
		temp++;
	}

	char *out = malloc(required_chars);
	temp = str + 1;
	int i = 0;

	while (*temp)
	{
		if (*temp == '\\')
		{
			temp++;
		}
		out[i] = *temp;
		i++;
		temp++;
	}
	out[i - 1] = *temp;

	return out;
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

size_t __struct_json___int_list_string_length(struct int_list list)
{
	size_t out = 2;
	char dummy[1];

	for (int i = 0; i < list.length; i++)
	{
		out += snprintf(dummy, sizeof(dummy), "%d", list.list[i]) + 1;
	}

	return out;
}

size_t __struct_json___float_list_string_length(struct float_list list)
{
	size_t out = 2;
	char dummy[1];

	for (int i = 0; i < list.length; i++)
	{
		out += snprintf(dummy, sizeof(dummy), "%f", list.list[i]) + 1;
	}

	return out;
}

size_t __struct_json___json_list_string_length(struct json_list list)
{
	size_t out = 2;

	for (int i = 0; i < list.length; i++)
	{
		out += strlen(list.list[i]) + 1;
	}

	return out;
}

size_t __struct_json___string_list_string_length(struct string_list list)
{
	size_t out = 2;

	for (int i = 0; i < list.length; i++)
	{
		out += __struct_json___required_chars_to_escape(list.list[i]) + 3;
	}

	return out;
}

/**
 * Adapted from http://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response,
 * originally written by Jerry Jeremiah, edited by zzeroo, modified by me
 */
char *http_request(const char *host, int portno, const char *method, const char *path, const char *data, const char **headers, size_t header_count)
{
	int i;

	struct hostent *server;
	struct sockaddr_in serv_addr;
	int sockfd, bytes, sent, received, total, message_size;
	char *message, *response;

	/* How big is the message? */
	message_size=0;
	if(!strcmp(method,"GET"))
	{
	    message_size+=strlen("%s %s%s%s HTTP/1.0\r\n");        /* protocol         */
	    message_size+=strlen(method);                         /* method           */
	    message_size+=strlen(path);                         /* path        */
	    if(data != NULL)
	        message_size+=strlen(data);                     /* query string   */
	    for(i=0;i<header_count;i++)                                    /* headers        */
	        message_size+=strlen(headers[i])+strlen("\r\n");
	    message_size+=strlen("\r\n");                          /* blank line     */
	}
	else
	{
	    message_size+=strlen("%s %s HTTP/1.0\r\n");
	    message_size+=strlen(method);                         /* method         */
	    message_size+=strlen(path);                         /* path           */
	    for(i=0;i<header_count;i++)                                    /* headers        */
	        message_size+=strlen(headers[i])+strlen("\r\n");
	    if(data!=NULL)
	        message_size+=strlen("Content-Length: %d\r\n")+10; /* content length */
	     message_size+=strlen("\r\n");                          /* blank line     */
	     if(data!=NULL)
	        message_size+=strlen(data);                     /* body           */
	}

	    /* allocate space for the message */
	    message=malloc(message_size);

	    /* fill in the parameters */
	    if(!strcmp(method,"GET"))
	    {
	        if(data != NULL)
	            sprintf(message,"%s %s%s%s HTTP/1.0\r\n",
	                strlen(method)>0?method:"GET",               /* method         */
	                strlen(path)>0?path:"/",                 /* path           */
	                strlen(data)>0?"?":"",                      /* ?              */
	                strlen(data)>0?data:"");                 /* query string   */
	        else
	            sprintf(message,"%s %s HTTP/1.0\r\n",
	                strlen(method)>0?method:"GET",               /* method         */
	                strlen(path)>0?path:"/");                /* path           */
	        for(i=0;i<header_count;i++)                                    /* headers        */
	            {strcat(message,headers[i]);strcat(message,"\r\n");}
	        strcat(message,"\r\n");                                /* blank line     */
	    }
	    else
	    {
	        sprintf(message,"%s %s HTTP/1.0\r\n",
	            strlen(method)>0?method:"POST",                  /* method         */
	            strlen(path)>0?path:"/");                    /* path           */
	        for(i=0;i<header_count;i++)                                    /* headers        */
	            {strcat(message,headers[i]);strcat(message,"\r\n");}
	        if(data != NULL)
	            sprintf(message+strlen(message),"Content-Length: %lu\r\n",strlen(data));
	        strcat(message,"\r\n");                                /* blank line     */
	        if(data != NULL)
	            strcat(message,data);                           /* body           */
	    }

	    /* What are we going to send? */
	    //printf("Request:\n%s\n",message);

	    /* create the socket */
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0) {free(message);return NULL;}//error("ERROR opening socket");

	    /* lookup the ip address */
	    server = gethostbyname(host);
	    if (server == NULL) {free(message);return NULL;}//error("ERROR, no such host");

	    /* fill in the structure */
	    memset(&serv_addr,0,sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(portno);
	    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

	    /* connect the socket */
	    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	    {free(message);return NULL;}//error("ERROR connecting");

	    /* send the request */
	    total = strlen(message);
	    sent = 0;

	    do {
	        bytes = write(sockfd,message+sent,total-sent);

	        if (bytes < 0)
	            return NULL;//error("ERROR writing message to socket");
	        if (bytes == 0)
	            break;
	        sent+=bytes;
	    } while (sent < total);

	    /* receive the response */
	    response = NULL;
	    received = 0;
	    char buf[4096];
	    int buf_size = 4096;

	    do {
	        bytes = read(sockfd,buf,buf_size);

	        if (bytes < 0)
	        {free(message);return NULL;}//error("ERROR reading response from socket");

	        response = realloc(response,received + bytes + 1);

	        for (int k = 0; k < bytes; k++)
	        {
	        	response[received] = buf[k];
	        	received++;
	        }
	    } while (bytes != 0);

	    response[received] = '\0';
	    /* close the socket */
	    close(sockfd);

	    /* return response */
	    free(message);

	    return response;//printf("Response:\n%s\n",response);

}

char *ajax_get_request(const char *url, const char *params)
{
	char *path = strchr(url,'/');
	char *host = malloc(path - url + 1);
	strncpy(host,url,path - url);
	host[path - url] = '\0';

	char *host_header = malloc(7 + strlen(host));
	strcpy(host_header, "Host: ");
	strcat(host_header, host);

	char *headers[] = {"Accept: application/json",
			           "Accept-Encoding: UTF-8",
					   "Accept-Language: en-US,en;q=0.8",
					   "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36",
					   host_header};

	char *response = http_request(host,80,"GET",path,params,(const char **) headers,5);
	char *json_part = strstr(response,"\r\n\r\n");
	if (strlen(json_part + 4) == 0)
	{
		free(response);
		return NULL;
	}
	else
	{
		char *out = malloc(strlen(json_part + 3));
		strcpy(out,json_part + 4);
		free(response);
		return out;
	}
}

char *ajax_post_request(const char *url, const char *data)
{
	char *path = strchr(url,'/');
		char *host = malloc(path - url + 1);
		strncpy(host,url,path - url);
		host[path - url] = '\0';

		char *host_header = malloc(7 + strlen(host));
		strcpy(host_header, "Host: ");
		strcat(host_header, host);

		char *headers[] = {"Accept: application/json",
				           "Accept-Encoding: UTF-8",
						   "Accept-Language: en-US,en;q=0.8",
						   "Content-Type: application/json",
						   "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36",
						   host_header};

		char *response = http_request(host,80,"POST",path,data,(const char **) headers,6);
		char *json_part = strstr(response,"\r\n\r\n");
		if (strlen(json_part + 4) == 0)
		{
			free(response);
			return NULL;
		}
		else
		{
			char *out = malloc(strlen(json_part + 3));
			strcpy(out,json_part + 4);
			free(response);
			return out;
		}
}

char *ajax_put_request(const char *url, const char *data)
{
	char *path = strchr(url,'/');
		char *host = malloc(path - url + 1);
		strncpy(host,url,path - url);
		host[path - url] = '\0';

		char *host_header = malloc(7 + strlen(host));
		strcpy(host_header, "Host: ");
		strcat(host_header, host);

		char *headers[] = {"Accept: application/json",
				           "Accept-Encoding: UTF-8",
						   "Accept-Language: en-US,en;q=0.8",
						   "Content-Type: application/json",
						   "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36",
						   host_header};

		char *response = http_request(host,80,"PUT",path,data,(const char **) headers,6);
		char *json_part = strstr(response,"\r\n\r\n");
		if (strlen(json_part + 4) == 0)
		{
			free(response);
			return NULL;
		}
		else
		{
			char *out = malloc(strlen(json_part + 3));
			strcpy(out,json_part + 4);
			free(response);
			return out;
		}
}

char *ajax_delete_request(const char *url, const char *data)
{
	char *path = strchr(url,'/');
		char *host = malloc(path - url + 1);
		strncpy(host,url,path - url);
		host[path - url] = '\0';

		char *host_header = malloc(7 + strlen(host));
		strcpy(host_header, "Host: ");
		strcat(host_header, host);

		char *headers[] = {"Accept: application/json",
				           "Accept-Encoding: UTF-8",
						   "Accept-Language: en-US,en;q=0.8",
						   "Content-Type: application/json",
						   "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36",
						   host_header};

		char *response = http_request(host,80,"DELETE",path,data,(const char **) headers,6);
		char *json_part = strstr(response,"\r\n\r\n");
		if (strlen(json_part + 4) == 0)
		{
			free(response);
			return NULL;
		}
		else
		{
			char *out = malloc(strlen(json_part + 3));
			strcpy(out,json_part + 4);
			free(response);
			return out;
		}
}
