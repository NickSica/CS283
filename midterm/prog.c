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

	char x[] = "This is a test";
	char y[] = "This is another test";
	char *token = strtok(x, " "); // Expecting "This"
	printf("%s\n", x);
	char *token3 = strtok(NULL, " "); // Expecting "is"
	char *token2 = strtok(y, " "); // Expecting "This"
	printf("%s\n", x);
	printf("%s\n", token);
	printf("%s\n", token2);
	printf("%s\n", token3);
	
	printf("%d\n", sizeof(int));
	int *f = (int *) malloc(4 * sizeof(int));
	int i;

	for(i = 0; i < 4; i++)
	{
		f[i] = i;
		printf("X[%d] = %d\n", i, x[i]);
	}

	/* Questions 8 and 9
	printf("%d\n", sizeof(x));
	f = (int *) realloc(x, sizeof(int)*8);
	printf("%d\n", sizeof(x));

	for(i = 0; i < 8; i++)
	{
		f[i] = i;
		printf("X[%d] = %d\n", i, x[i]);
	}

	free(f);
	*/

	return 0;
}









