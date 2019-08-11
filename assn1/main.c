
#include "parser.h"
#include "table.h"

int main(int argc, char *argv[])
{
    tableNode *table1 = allocTable(sizeof(char), sizeof(char), sizeof(char));
    
    char input[100];
    printf("Input database directory path: ");
    fgets(input, 100, stdin);
    if(strcmp(input, "\n") == 0)
    {
	printf("No directory specified.\n");
	return -1;
    }
    char *input2 = strtok(input, "\n");
    char *dbDir = strcat(input2, "/");

    struct stat info;
    if(stat(dbDir, &info) == 0)
	printf("Opening database.\n");
    else if(mkdir(dbDir, 0777) == -1)
    {
	printf("Database creation failed, exiting.\n");
	return -1;
    }
    else
	printf("Database created.\n");

    int fd = Open(argv[1], O_CREAT | O_RDONLY, 0);
    rio_t rio;
    Rio_readinitb(&rio, fd);
    while(1)
    {
	uint8_t isChanged = 0;
	char input[200];
	
	if(argc > 1)
	{
	    if(fd != -1)
	    {
		int isNotDone = Rio_readlineb(&rio, input, 200);
		if(strcmp(input, "\n") != 0)
		    printf("Read command: %s", input);
		if(isNotDone == 0)
		    break;
	    }
	}
	else
	{
	    printf("Enter a command:\n");
	    fgets(input, 200, stdin);
	    if(strcmp(input, "exit") == 0)
		break;
	}

	command *cmd = malloc(sizeof(command));
	cmd->fieldNames = malloc(sizeof(char *)*2);
	cmd->fileNames = malloc(sizeof(char *)*2);
	cmd->vals = malloc(sizeof(char *)*2);
	cmd->operation = malloc(sizeof(char)*15);
	for(int i = 0; i < 2; i++)
	{
	    cmd->vals[i] = malloc(sizeof(input));
	    cmd->fieldNames[i] = malloc(sizeof(input));
	    cmd->fileNames[i] = malloc(sizeof(input));
	}
        parseSQL(input, cmd);
	table1 = openTable(table1, cmd->lengths, cmd->longestVal, cmd->fileNames[0], dbDir, cmd->operation);

	if(strcmp(cmd->operation, "SELECT WHERE") == 0)          // SELECT * FROM TableName WHERE Field1="value"
	{
	    int numNodes = 0;
	    tableNode **node = findNode(table1, cmd->vals[0], cmd->fieldNames[0], &numNodes);
	    int i;
	    if(node == NULL)
		printf("No node matches that!\n");
	    else
		for(i = 0; i <= numNodes; i++)
		    printNode(node[i]);
	}
	else if(strcmp(cmd->operation, "SELECT JOIN") == 0)     // SELECT * FROM TableName JOIN TableName2 ON TableName.Field1 = TableName2.Field2
	{

	    isChanged = 1;
	}
	else if(strcmp(cmd->operation, "DELETE") == 0)          // DELETE FROM TableName WHERE Field1="value"
	{
	    int numNodes;
       	    tableNode **node = findNode(table1, cmd->vals[0], cmd->fieldNames[0], &numNodes);
	    int i;
	    for(i = 0; i <= numNodes; i++)
		if(node[i] != NULL)
		{
		    tableNode *next = node[i]->next;
		    tableNode *prev = node[i]->prev;
		    if(prev != NULL)
			prev->next = next;
		    if(next != NULL)
			next->prev = prev;
		    isChanged = 1;
		}
	}
	else if(strcmp(cmd->operation, "UPDATE") == 0)          // UPDATE TableName SET Field1="new value" WHERE Field2="value"
	{
	    int numNodes;
	    tableNode **node = findNode(table1, cmd->vals[1], cmd->fieldNames[1], &numNodes);
	    for(int i = 0; i <= numNodes; i++)
	    {
		if(node[i] != NULL)
		{		 
		    int fieldLoc = getFieldLoc(node[i]->length, table1->vals, cmd->fieldNames[0]);
		    if(fieldLoc != -1)
		    {
			strcpy(node[i]->vals[fieldLoc], cmd->vals[0]);
			isChanged = 1;
		    }
		}
	    }
	}
	else if(strcmp(cmd->operation, "INSERT") == 0)          // INSERT INTO TableName (Field1="value", Field2="value", ...)
	{
	    if(cmd->lengths == table1->length)
	    {
		tableNode *tmpNode = allocTable(cmd->lengths, cmd->longestVal, sizeof(cmd->fileNames[0]));
		tmpNode->next = NULL;
		tmpNode->length = cmd->lengths;
		strcpy(tmpNode->fileName, cmd->fileNames[0]);
		for(int i = 0; i < cmd->lengths; i++)
		{
		    int fieldLoc = getFieldLoc(table1->length, table1->vals, cmd->fieldNames[i]);
		    if(fieldLoc == -1)
		    {
			printf("That field doesn't exist. Exiting.\n");
		        return -1;
		    }
		    else
			strcpy(tmpNode->vals[fieldLoc], cmd->vals[i]);
		}
		
		tableNode *node = table1;
		while(node != NULL)
		{
		    if(node->next == NULL)
		    {
			node->next = tmpNode;
			tmpNode->prev = node;
			break;
		    }
		    node = node->next;
		}
		node = NULL;
		isChanged = 1;
	    }
	    else
		printf("Inputted field amount doesn't match the table(Don't forget to put in the blank values).\n");
		
	}
	else if(strcmp(cmd->operation, "CREATE") == 0)          // CREATE TABLE TableName FIELDS [Field1, Field2, ...]
	{
       	    for(int i = 0; i < cmd->lengths; i++)
		strcpy(table1->vals[i], cmd->vals[i]);
	    strcpy(table1->fileName, cmd->fileNames[0]);
	    table1->length = cmd->lengths;
	    isChanged = 1;
	}
	else if(strcmp(cmd->operation, "DROP") == 0)             // DROP TABLE TableName
	{
	    char *name = strcat(dbDir, cmd->fileNames[0]);
	    if(remove(name) == 0)
		printf("Table deletion succeeded.\n");
	    else
		printf("Table deletion failed.\n");
	    
	    if(strcmp(table1->fileName, cmd->fieldNames[0]) == 0)
	    {
		free(table1->fileName);
		table1->fileName = malloc(sizeof("This has been deleted.")+2);
		strcpy(table1->fileName, "This has been deleted.");
	    }	       
	}

	if(isChanged == 1)
	    writeTable(table1, dbDir);

	int len = 2;
	if(strcmp(cmd->operation, "INSERT") == 0 || strcmp(cmd->operation, "CREATE") == 0)
	    len = cmd->lengths;
	for(int i = 0; i < len; i++)
	{
	    free(cmd->vals[i]);
	    free(cmd->fieldNames[i]);
	}
	free(cmd->operation);
	free(cmd->fieldNames);
	free(cmd->fileNames[0]);
	free(cmd->fileNames[1]);
	free(cmd->fileNames);
	free(cmd->vals);
	free(cmd);
    }
    Close(fd);
    freeTable(table1);
    return 0;
}
















