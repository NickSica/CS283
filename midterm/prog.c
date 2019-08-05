#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	/*
	char x[70] = "Helloooooo!";
	strncpy(x, "Hello.", strlen("Hello.")+1);
	int xLength = strlen(x);
	int xSize = sizeof(x);
	int length = strlen("Hello.")+1;
	size_t size = sizeof("Hello.");
	printf(x);
	printf("%d %d %d %d", xLength, xSize, length, size);
	*/

	/*	
	char *x = "This is a test";
	char *y = "This is another test";
	char *token = strtok(x, " "); // Expecting "This"
	//char *token2 = strtok(y, " "); // Expecting "This"
	//char *token3 = strtok(x, " "); // Expecting "is"
	//printf(token);
	printf("\n");
	//printf(token2);
	printf("\n");
	//printf(token3);	
	*/
	
	printf("%d\n", sizeof(int));
	int *x = (int *) malloc(4 * sizeof(int));
	int i;

	for(i = 0; i < 4; i++)
	{
		x[i] = i;
		printf("X[%d] = %d\n", i, x[i]);
	}
	
	printf("%d\n", sizeof(x));
	x = (int *) realloc(x, sizeof(int)*4);
	printf("%d\n", sizeof(x));

	for(i = 0; i < 8; i++)
	{
		x[i] = i;
		printf("X[%d] = %d\n", i, x[i]);
	}

	free(x);


	return 0;
}
