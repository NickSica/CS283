#include "csapp.h"

char *read_request(int *connfd, int listenfd)
{
    struct sockaddr_in client_addr;
    int client_len;
    char line[1024];
    *connfd = Accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
    struct hostent *hp = Gethostbyaddr((const char *)&client_addr, sizeof(client_addr.sin_addr.s_addr), AF_INET);
    char *haddrp = inet_ntoa(client_addr.sin_addr);
    printf("Server connected to %s (%s)\n", hp->h_name, haddrp);
    rio_t rio;
    Rio_readinitb(&rio, *connfd);
    Rio_readlineb(&rio, line, sizeof(line));
    strtok(line, " ");
    char *page = strtok(NULL, " ");
    return page;
}

void send_response(char *page, int connfd)
{
    printf("SEND RESPONSE!\n");
    FILE *open_file = fopen(page, "r");
    fseek(open_file, 0, SEEK_END);
    long file_size = ftell(open_file);
    printf("FILE SIZE: %ld\n", file_size);
    fseek(open_file, 0, SEEK_SET);
    char *buf = malloc(sizeof(file_size));
    fread(buf, 1, file_size, open_file);
    char size[260];
    sprintf(size, "%ld\n", file_size+1);
    printf("BUF: %s\n", buf);
    Rio_writen(connfd, size, strlen(size));
    Rio_writen(connfd, buf, strlen(buf));
}

int main(int argc, char **argv)
{
    int connfd;
    int listenfd = Open_listenfd(atoi(argv[1]));
    while(1)
    {
	char *page = read_request(&connfd, listenfd);
	printf("PAGE: %s\n", page);
	send_response(page, connfd);
    }
    Close(listenfd);
    exit(0);
}



