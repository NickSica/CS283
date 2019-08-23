#include "messageFunctions.h"

void *send_message(void *arg)
{
    arguments *args = (arguments *)arg;
    while(1)
    {
	printf("Type, enter .bye to quit\n");
	char buf[MAX_LINE];
	fgets(buf, MAX_LINE, stdin);
	buf[strlen(buf) - 1] = '\0';

	if(strcmp(buf, ".bye") == 0)
	    exit(0);
	
	printf("ENCRYPTED %s to ", buf);
	for(int i = 0; i < (int)strlen(buf) - 1; i++)
	{
	    long encrypted = encrypt(buf[i], args->key, args->c);
	    char encrypted_char[256];
	    if(i == (int)strlen(buf) - 2)
		sprintf(encrypted_char, "%ld\n", encrypted);
	    else
		sprintf(encrypted_char, "%ld ", encrypted);
	    printf("%s", encrypted_char);
	    Rio_writen(args->connfd, encrypted_char, strlen(encrypted_char));
	}
	Rio_writen(args->connfd, buf, strlen(buf));
    }
    return NULL;
}

void *read_message(void *arg)
{
    arguments *args = (arguments *)arg;
    char line[1024];
    rio_t rio;
    Rio_readinitb(&rio, args->connfd);
    while(1)
    {
	int status = Rio_readlineb(&rio, line, sizeof(line));
	if(status == -1 || status == 0)
	{
	    printf("Connection Error.\n");
	    free(args);
	    exit(1);
	}
	else
	{
	    printf("DECRYPTED %s to ", line);
	    char *encrypted_char = malloc(strlen(line)+1);
	    encrypted_char = strtok(line, " \n");	    
	    while(encrypted_char != NULL)
	    {
		uint8_t decrypted_char = decrypt(atol(encrypted_char), args->key, args->c);
		printf("%c", decrypted_char);
		encrypted_char = strtok(NULL, " \n");
	    }
	    printf("\n");
	}
    }
    return NULL;
}




