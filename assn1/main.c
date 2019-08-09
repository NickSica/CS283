#include <stdio.h>
#include <stdlib.h>

#include "csapp.h"
#include "parser.h"

struct tableNode;
typedef struct tableNode tableNode;

struct tableNode {
    char **vals;
    char *fileName;
    tableNode *next;
    tableNode *prev;
    int length;
};

tableNode *openTable(char *tableName, char *dbDir, int fd)
{
    char *path = strcat(dbDir, tableName);
    fd = Open(path, O_CREAT | O_RDWR, 0);
    int n;
    rio_t rio;
    char buf[MAXLINE];
    
    Rio_readinitb(&rio, fd);
    if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
	int length = 0;
        char *tmpBuf = buf;
	char *tmp = strtok(tmpBuf, ",");
	char **fixedVals;
	while(tmp != NULL)
	{
	    fixedVals[length] = tmp;
	    length++;
	    tmp = strtok(NULL, ",");
	}
	tableNode *head = {fixedVals, tableName, NULL, NULL, length};
	tableNode *prev = head;
	
	while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
	{
	    fixedVals = NULL;
	    tmpBuf = buf;
	    tmp = strtok(tmpBuf, ",");
	    length = 0;
	    while(tmp != NULL)
	    {
		fixedVals[length] = tmp;
		length++;
		tmp = strtok(NULL, ",");
	    }
	    
	    tableNode *tmpNode = {fixedVals, tableName, NULL, prev, length};
	    prev->next = tmpNode;
	    prev = tmpNode;
	}
    }	
    else
    {
	tableNode *head = {NULL, tableName, NULL, NULL, 0};
	return head;
    }
}

tableNode *createTable(char *tableName, command *cmd, uint8_t tableNum)
{
    tableNode *headNode = {cmd->fieldNames, cmd->fileNames[tableNum-1], NULL, NULL, cmd->lengths};
}

int getFieldLoc(int length, char **vals, char *fieldName)
{
    int fieldLoc;
    int i;
    for(i = 0; i < length; i++)
    {
	if(strcmp(&vals[i], fieldName))
	{
	    fieldLoc = i;
	    break;
	}
    } 
}

tableNode *findNode(tableNode *table, char *val, char *fieldName)
{
    tableNode *find = table->next;
    int fieldLoc = getFieldLoc(table->length, table->vals, fieldName);

    // Find where field="val" using the location from the head array
    while(find != NULL)
    {
	if(strcmp(find->vals[fieldLoc], val))
	    break;
	find = find->next;
    }
    return find;
}

void printNode(tableNode *node)
{
    if(node == NULL)
	printf("Value not found.\n");
    else
    {
	int i;
	for(i = 0; i < node->length-2; i++)
	    printf("%s,", node->vals[i]);
	printf("%s", node->vals[node->length-1]);
    }	       
}

int main(void)
{
    command *cmd;
    tableNode *table1 = malloc(sizeof(tableNode));
    tableNode *table2 = malloc(sizeof(tableNode));
    table1->next = NULL;
    table1->prev = NULL;
    table2->next = NULL;
    table2->prev = NULL;

    int file = -1;
    char dbDir[100];
    printf("Input database directory path: ");
    fgets(dbDir, 60, stdin);
    
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
	char input[100];
	cmd = malloc(sizeof(input));
	printf("Enter a command:\n");
	fgets(input, 60, stdin);
	if(strcmp(input, "exit") == 0)
	    break;
	
	parseSQL(input, cmd);
	if(strcmp(cmd->operation, "SELECT WHERE") == 0)          // SELECT * FROM TableName WHERE Field1="value"
	{
	    if(strcmp(table1->fileName, cmd->fileNames[0]))
	        table1 = openTable(cmd->fileNames[0], dbDir, file);
	    tableNode *find = findNode(table1, cmd->vals[0], cmd->fieldNames[0]);
	    printNode(find);
	}
	else if(strcmp(cmd->operation, "SELECT JOIN") == 0)     // SELECT * FROM TableName JOIN TableName2 ON TableName.Field1 = TableName2.Field2
	{

	}
	else if(strcmp(cmd->operation, "DELETE") == 0)          // DELETE FROM TableName WHERE Field1="value"
	{
	    
	}
	else if(strcmp(cmd->operation, "UPDATE") == 0)          // UPDATE TableName SET Field1="new value" WHERE Field2="value"
	{

	}
	else if(strcmp(cmd->operation, "INSERT") == 0)          // INSERT INTO TableName (Field1="value", Field2="value", ...)
	{
	    if(cmd->lengths == table1->length)
	    {
		char **vals = malloc(sizeof(cmd->vals));
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
	    }
	    else
		printf("Inputted field amount doesn't match the table(Don't forget to put in the blank values).");
		
	}
	else if(strcmp(cmd->operation, "CREATE") == 0)          // CREATE TABLE TableName FIELDS [Field1, Field2, ...]
	{
	    int i = 0;
	    for(i = 0; i < cmd->lengths; i++)
		table1->vals[i] = cmd->fieldNames[i];
	    table1->fileName = cmd->fileNames[0];
	    table1->length = cmd->lengths;
	    tableNode *newTable = malloc(sizeof(tableNode));
	    table1->next = newTable;
	}
	else if(strcmp(cmd->operation, "DROP") == 0)             // DROP TABLE TableName
	{
	    char *name = strcat(dbDir, cmd->fileNames[0]);
	    if(remove(name) == 0)
		printf("Table deletion succeeded.");
	    else
		printf("Table deletion failed.");

	    if(strcpy(table1->fileName, cmd->fieldNames[0]) == 0)
		table1 = NULL;

	    if(strcpy(table2->fileName, cmd->fieldNames[0]) == 0)
		table2 = NULL;
	}
    }

    free(cmd);

    while(table1 != NULL)
    {
	tableNode *tempTable = table1->next;
	free(table1);
	table1 = tempTable;
    }

    while(table2 != NULL)
    {
	tableNode *tempTable = table2->next;
	free(table2);
	table1 = tempTable;
    }
    
    return 0;
}









