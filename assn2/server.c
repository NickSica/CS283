#include "server.h"

int main(int argc, char **argv)
{
    if(argc < 5)
    {
	printf("Incorrect Usage.\n");
	return 1;
    }

    int port = atoi(argv[1]);
    long e = atol(argv[2]);
    long c = atol(argv[3]);
    long d = atol(argv[4]);
    long dc = atol(argv[5]);
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    printf("Waiting for a client...\n");
    struct sockaddr_in client_addr;
    unsigned int client_len;
    int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
    struct hostent *hp = Gethostbyaddr((const char *)&client_addr, sizeof(client_addr.sin_addr.s_addr), AF_INET);
    char *haddrp = inet_ntoa(client_addr.sin_addr);
    printf("Server connected to %s (%s)\n", hp->h_name, haddrp);

    arguments *send_args = malloc(sizeof(arguments));
    pthread_t tid1, tid2;
    send_args->connfd = connfd;    
    send_args->key = e;
    send_args->c = c;
    pthread_create(&tid1, NULL, send_message, (void *)send_args);

    arguments *read_args = malloc(sizeof(arguments));
    read_args->connfd = connfd;
    read_args->key = d;
    read_args->c = dc;
    pthread_create(&tid2, NULL, read_message, (void *)read_args);
    pthread_join(tid1, NULL);

    Close(connfd);
    Close(listenfd);
    free(send_args);
    free(read_args);
    exit(0);
}



