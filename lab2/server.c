#include "csapp.h"
#include <dirent.h>

char *read_request(int *connfd, int listenfd)
{
    struct sockaddr_in client_addr;
    unsigned int client_len;
    char line[1024];
    rio_t rio;
    *connfd = Accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
    struct hostent *hp = Gethostbyaddr((const char *)&client_addr, sizeof(client_addr.sin_addr.s_addr), AF_INET);
    char *haddrp = inet_ntoa(client_addr.sin_addr);
    printf("Server connected to %s (%s)\n", hp->h_name, haddrp);
    
    while(1)
    {	
	Rio_readinitb(&rio, *connfd);
	int status = Rio_readlineb(&rio, line, sizeof(line));
	if(status == -1 || status == 0)
	{
	    printf("Connection Error.\n");
	    return NULL;
	}
	char *cmd = strtok(line, " ");
	char *page = strtok(NULL, " ");
	char *return_page = malloc(strlen(page)+1);
	strcpy(return_page, page);
	char *http_version = strtok(NULL, "\r\n");
	if(strcmp(http_version, "HTTP/1.1") == 0 && strcmp(cmd, "GET") == 0)
	    return return_page;
	else
	    Rio_writen(*connfd, "HTTP/1.1 400 Bad Request\r\n", strlen("HTTP/1.1 400 Bad Request\r\n"));
    }
}

void send_response(char *page, int connfd)
{
    FILE *open_file = fopen(page, "r");
    if(open_file == NULL)
	Rio_writen(connfd, "HTTP/1.1 404 Page Not Found\r\n", strlen("HTTP/1.1 404 Page Not Found\r\n"));
    else
    {
	Rio_writen(connfd, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"));
	
	fseek(open_file, 0, SEEK_END);
	long file_size = ftell(open_file);
	fseek(open_file, 0, SEEK_SET);

	char size[260];
	sprintf(size, "Content-Length: %ld\r\n", file_size);
	Rio_writen(connfd, size, strlen(size));
	
	char *buf = malloc(file_size);
	fread(buf, 1, file_size, open_file);
	Rio_writen(connfd, buf, strlen(buf));
    }
}

int main(int argc, char **argv)
{
    int connfd;
    int listenfd = Open_listenfd(atoi(argv[1]));
    while(1)
    {
	char *page = read_request(&connfd, listenfd);
	if(page != NULL)
	{
	    send_response(page, connfd);
	    free(page);
	}
	Close(connfd);
    }
    Close(listenfd);
    exit(0);
}



