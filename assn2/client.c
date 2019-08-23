#include "client.h"

int main(int argc, char **argv)
{
    if(argc < 6)
    {
	printf("Incorrect Usage.\n");
	return -1;
    }

    char *server = argv[1];
    int port = atoi(argv[2]);
    long e = atol(argv[3]);
    long c = atol(argv[4]);
    long d = atol(argv[5]);
    long dc = atol(argv[6]);
    int clientfd = 0;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
	printf("Error: Couldn't create socket.\n");
	return 1;
    }
    
    if(inet_pton(AF_INET, server, &serv_addr.sin_addr) <= 0)
    {
	printf("Error: pton.\n");
	return 1;
    }

    if(connect(clientfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
	printf("Error: Connection failed.\n");
	return 1;
    }

    arguments *send_args = malloc(sizeof(arguments));
    pthread_t tid1, tid2;
    send_args->connfd = clientfd;    
    send_args->key = e;
    send_args->c = c;
    pthread_create(&tid1, NULL, send_message, (void *)send_args);

    arguments *read_args = malloc(sizeof(arguments));
    read_args->connfd = clientfd;
    read_args->key = d;
    read_args->c = dc;
    pthread_create(&tid2, NULL, read_message, (void *)read_args);
    pthread_join(tid1, NULL);
    
    Close(clientfd);
    free(send_args);
    free(read_args);
    return 0;
}








