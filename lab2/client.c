#include "csapp.h"
#define MAX_LINE 256

void send_request(int clientfd)
{
    char buf[MAX_LINE];
    uint8_t numTimes = 0;
    fgets(buf, MAX_LINE, stdin);
    while(1)
    {
	if(buf[0] != '\r' && buf[0] != '\n')
	{
	    Rio_writen(clientfd, buf, strlen(buf));
	    numTimes = 1;
	}
        else if(numTimes == 1)
	    break;
	fgets(buf, MAX_LINE, stdin);
    }
    //fgets(buf, MAX_LINE, stdin);
    //Rio_writen(clientfd, buf, strlen(buf));
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
    if(status == 404)
    {
	printf("Page not found.\n");
    }
    else if(status == 400)
    {
	printf("Bad Request.");
    }
    else
    {
	unsigned long bytes_to_read;
	uint8_t isChunked = 0;
	Rio_readlineb(&rio, line, sizeof(line));
	/*
	while(line[0] != '\r' && line[0] != '\n')
	{
	    printf("BEGINNING OF LINE:%c\n", line[0]);
	    //printf("LINE:%s\n", line);
	    char *key = strtok(line, ":");
	    char *val = strtok(NULL, "\r\n");
	    printf("KEY:%sl\n", key);
	    
	    if(val != NULL && *val == ' ')
		val += 1;
	    printf("VAL:%sl\n", val);
	    
	    if(strcmp(key, "Content-Length") == 0)
		bytes_to_read = atoi(val);
	    else if(strcmp(key, "Transfer-Encoding") == 0)
		if(strcmp(val, "chunked") == 0)
		    isChunked = 1;
	    Rio_readlineb(&rio, line, sizeof(line));
	}
	
	Rio_readlineb(&rio, line, sizeof(line));
	bytes_to_read = strtol(line, 0, 16);
	*/
	char *key = strtok(line, ":");
	char *val = strtok(NULL, "\r\n");
	if(val != NULL && *val == ' ')
	    val += 1;
	bytes_to_read = strtol(val, 0, 10);
	if(isChunked == 1)
	{
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

    char *server = argv[1];
    int port = atoi(argv[2]);
    //int clientfd = Open_clientfd(server, port);
    while(1)
    {
	int clientfd = Open_clientfd(server, port);
	send_request(clientfd);
	read_response(clientfd);
	Close(clientfd);
    }
    //Close(clientfd);
    exit(0);
}








