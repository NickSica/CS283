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
	for(int i = 0; i < (int)strlen(buf); i++)
	{
	    long encrypted = encrypt(buf[i], args->key, args->c);
	    char encrypted_char[256];
	    if(i == (int)strlen(buf) - 1)
		sprintf(encrypted_char, "%ld\n", encrypted);
	    else
		sprintf(encrypted_char, "%ld ", encrypted);
	    printf("%s", encrypted_char);
	    Rio_writen(args->connfd, encrypted_char, strlen(encrypted_char));
	}
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
	size_t line_len = strlen(line) - 1;
	if(line[line_len] == '\r' || line[line_len] == '\n')
	    line[line_len] = '\0';
	    
	if(status == -1 || status == 0)
	{
	    printf("Connection Error.\n");
	    return NULL;
	}
	else
	{
	    printf("DECRYPTED %s to ", line);
	    char *message = malloc(strlen(line));
	    strcpy(message, "");
	    char *encrypted_char = strtok(line, " \n");	    
	    while(encrypted_char != NULL)
	    {
		char decrypted_char[2];
		decrypted_char[0] = decrypt(atol(encrypted_char), args->key, args->c);
		decrypted_char[1] = '\0';
		strcat(message, decrypted_char);
// 		printf("DECRYPTED %s to %c\n", encrypted_char, decrypted_char);
		encrypted_char = strtok(NULL, " \n");
	    }
	    printf("%s\n", message);
	    free(message);
	}
    }
    return NULL;
}




