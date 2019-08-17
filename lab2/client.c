#include "csapp.h"
#define MAX_LINE 256

void send_request(int clientfd)
{
    char buf[MAX_LINE];
    fgets(buf, MAX_LINE, stdin);
    Rio_writen(clientfd, buf, strlen(buf));
    fgets(buf, MAX_LINE, stdin);
    Rio_writen(clientfd, buf, strlen(buf));
}

void read_response(int clientfd)
{
    char line[1024];
    rio_t rio;
    Rio_readinitb(&rio, clientfd);
    Rio_readlineb(&rio, line, sizeof(line));
    unsigned long bytes_to_read = strtol(line, NULL, 10);
    char *buf = malloc(bytes_to_read);
    Rio_readnb(&rio, buf, bytes_to_read);
    //printf("%s", buf);
    free(buf);
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
    int clientfd = Open_clientfd(server, port);
    while(1)
    {
	send_request(clientfd);
	read_response(clientfd);
    }
    Close(clientfd);
    exit(0);
}
