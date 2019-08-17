#include "csapp.h"
#define MAX_LINE 256

void send_request(int clientfd, char *page)
{
    char *buf = malloc(MAX_LINE);
    sprintf(buf, "GET %s HTTP/1.1\r\n\r\n", page);
    Rio_writen(clientfd, buf, strlen(buf));
    free(buf);
}

void read_response(int clientfd)
{
    char line[1024];
    rio_t rio;
    Rio_readinitb(&rio, clientfd);
    Rio_readlineb(&rio, line, sizeof(line));
    strtok(line, " ");
    int status = atoi(strtok(NULL, " "));
    strtok(line, "\n");
    if(status != 200)
    {
	printf("Error connecting to website page.");
    }
    else
    {
	unsigned long bytes_to_read;
	uint8_t isChunked = 0;
	Rio_readlineb(&rio, line, sizeof(line));
	while(line[0] != '\r' && line[0] != '\n')
	{
	    char *key = strtok(line, ":");
	    char *val = strtok(NULL, "\r\n");
	    if(strcmp(key, "Content-Length") == 0)
		bytes_to_read = atoi(val);
	    else if(strcmp(key, "Transfer-Enconding"))
		if(strcmp(val, " chunked"))
		    isChunked = 1;
	    Rio_readlineb(&rio, line, sizeof(line));
	}
	
	Rio_readlineb(&rio, line, sizeof(line));
	bytes_to_read = strtol(line, 0, 16);
	if(isChunked == 1)
	    while(bytes_to_read != 0)
	    {
		char *buf = malloc(bytes_to_read);
		Rio_readnb(&rio, buf, bytes_to_read);
		printf("%s", buf);
		Rio_readlineb(&rio, line, sizeof(line));
		Rio_readlineb(&rio, line, sizeof(line));
		bytes_to_read = strtol(line, 0, 16);
		free(buf);
	    }
	else
	{
	    char *buf = malloc(bytes_to_read);
	    Rio_readnb(&rio, buf, bytes_to_read);
	    printf("%s", buf);
	    free(buf);
	}
	
    }
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
	printf("Incorrect Usage.");
	return -1;
    }

    char *website = argv[1];
    char *page = argv[2];
    int clientfd = Open_clientfd(website, 80);
    send_request(clientfd, page);
    read_response(clientfd);
    Close(clientfd);
    exit(0);
}






