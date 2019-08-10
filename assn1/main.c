#include "parser.h"
#include "table.h"

int main(void)
{
    command *cmd;
    tableNode *table1 = malloc(sizeof(tableNode));
    tableNode *table2 = malloc(sizeof(tableNode));

    char input[100];
    printf("Input database directory path: ");
    fgets(input, 100, stdin);
    char *dbDir = strcat(input, "/");
    
    struct stat stat;
    Stat(dbDir, &stat);

    if(S_ISDIR(stat.st_mode))
	printf("Opening database.");
    else if(mkdir(dbDir, 0777 == -1))
    {
	printf("Database creation failed, exiting.");
	return -1;
    }
    else
	printf("Database created.");

    while(1)
    {
	uint8_t isChanged = 0;
	char input[100];
	cmd = malloc(sizeof(input));
	printf("Enter a command:\n");
	fgets(input, 60, stdin);
	if(strcmp(input, "exit") == 0)
	    break;
	
	parseSQL(input, cmd);
	table1 = openTable(table1, cmd->fileNames[0], dbDir);
	
	
	if(table1 == NULL)
	    	printf("That table doesn't exist(Use a CREATE command first).");
	else if(strcmp(cmd->operation, "SELECT WHERE") == 0)          // SELECT * FROM TableName WHERE Field1="value"
	{
	    tableNode **node = findNode(table1, cmd->vals[0], cmd->fieldNames[0]);
	    int i;
	    for(i = 0; i < (int)(sizeof(node)/sizeof(node[0])); i++)
		if(node[0] != NULL)
		    printNode(node[i]);
	}
	else if(strcmp(cmd->operation, "SELECT JOIN") == 0)     // SELECT * FROM TableName JOIN TableName2 ON TableName.Field1 = TableName2.Field2
	{

	    isChanged = 1;
	}
	else if(strcmp(cmd->operation, "DELETE") == 0)          // DELETE FROM TableName WHERE Field1="value"
	{
       	    tableNode **node = findNode(table1, cmd->vals[0], cmd->fieldNames[0]);
	    int i;
	    for(i = 0; i < (int)(sizeof(node)/sizeof(node[0])); i++)
		if(node[0] != NULL)
		{
		    node[i]->prev->next = node[i]->next;
		    node[i]->next->prev = node[i]->prev;
		    isChanged = 1;
		}
	}
	else if(strcmp(cmd->operation, "UPDATE") == 0)          // UPDATE TableName SET Field1="new value" WHERE Field2="value"
	{
	    tableNode **node = findNode(table1, cmd->vals[1], cmd->fieldNames[1]);
	    if(node[0] != NULL)
	    {
		int i;
		for(i = 0; i < (int)(sizeof(node)/sizeof(node[0])); i++)
		{
		    int fieldLoc = getFieldLoc(node[i]->length, table1->vals, cmd->fileNames[0]);
		    if(fieldLoc != -1)
		    {
			node[i]->vals[fieldLoc] = cmd->vals[0];
			isChanged = 1;
		    }
		}
	    }
	}
	else if(strcmp(cmd->operation, "INSERT") == 0)          // INSERT INTO TableName (Field1="value", Field2="value", ...)
	{
	    if(cmd->lengths == table1->length)
	    {
		char *vals[cmd->lengths];
		int i;
		for(i = 0; i < cmd->lengths; i++)
		{
		    int fieldLoc = getFieldLoc(table1->length, table1->vals, cmd->fieldNames[i]);
		    vals[fieldLoc] = cmd->vals[i];
		}
		
		tableNode *tmpNode = malloc(sizeof(input));
		tmpNode->next = NULL;
		tmpNode->length = cmd->lengths;
		tmpNode->vals = vals;
		tmpNode->fileName = cmd->fileNames[0];
	        
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
		isChanged = 1;
	    }
	    else
		printf("Inputted field amount doesn't match the table(Don't forget to put in the blank values).");
		
	}
	else if(strcmp(cmd->operation, "CREATE") == 0)          // CREATE TABLE TableName FIELDS [Field1, Field2, ...]
	{
	    char confirm = 'n';
	    struct stat stat;
	    if(S_ISREG(stat.st_mode))
	    {
		while(confirm != 'y' || confirm != 'n')
		{
		    printf("\nThis file exists are you sure you want to overwrite it?[y/n] ");
		    confirm = getchar();
		}
	    }

	    if(confirm == 'y')
	    {
		int i = 0;
		for(i = 0; i < cmd->lengths; i++)
		    table1->vals[i] = cmd->fieldNames[i];
		table1->fileName = cmd->fileNames[0];
		table1->length = cmd->lengths;
		tableNode *newTable = malloc(sizeof(tableNode));
		table1->next = newTable;
		isChanged = 1;
	    }
	}
	else if(strcmp(cmd->operation, "DROP") == 0)             // DROP TABLE TableName
	{
	    char *name = strcat(dbDir, cmd->fileNames[0]);
	    if(remove(name) == 0)
		printf("Table deletion succeeded.");
	    else
		printf("Table deletion failed.");

	    if(strcpy(table1->fileName, cmd->fieldNames[0]) == 0)
	    {
		freeTable(table1);
		table1 = malloc(sizeof(tableNode));   
	    }
	       
	    if(strcpy(table2->fileName, cmd->fieldNames[0]) == 0)
	    {
		freeTable(table2);
		table2 = malloc(sizeof(tableNode));	
	    }
	}

	if(isChanged == 1)
	    writeTable(table1, dbDir);
    }

    free(cmd);
    freeTable(table1);
    freeTable(table2);
    return 0;
}
