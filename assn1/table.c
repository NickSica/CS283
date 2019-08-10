#include "table.h"

void writeTable(tableNode *table, char *dbDir)
{

    char *path = strcat(dbDir, table->fileName);
    int fd = Open(path, O_CREAT | O_WRONLY, 0);
    tableNode* currNode = table;
    while(currNode != NULL)
    {
	int i;
	for(i = 0; i < currNode->length; i++)
	{
	    if(i == currNode->length - 1)
	    {
		char *buf = strcat(currNode->vals[i], "\n");
		Rio_writen(fd, buf, sizeof(buf));
	    }
	    else
	    {
		char *buf = strcat(currNode->vals[i], ",");
		Rio_writen(fd, buf, sizeof(buf));
	    }
	}
	currNode = currNode->next;
    }
    Close(fd);
}

tableNode *openTable(tableNode *table, char *cmdFileName, char *dbDir)
{
    if(strcmp(table->fileName, cmdFileName) != 0)
    {
	freeTable(table);
	char *path = strcat(dbDir, cmdFileName);
	int fd = Open(path, O_CREAT | O_RDONLY, 0);
	int n;
	rio_t rio;
	char buf[MAXLINE];
    
	Rio_readinitb(&rio, fd);
	if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
	{
	    tableNode *head = malloc(sizeof(buf));
	    int length = 0;
	    char *tmp = strtok(buf, ",");
	    while(tmp != NULL)
	    {
		head->vals[length] = tmp;
		length++;
		tmp = strtok(NULL, ",");
	    }
	    strcpy(head->fileName, cmdFileName);
	    head->next = NULL;
	    head->prev = NULL;
	    head->length = length;
	    tableNode *prev = head;
	
	    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
	    {
		tableNode *tmpNode = malloc(sizeof(buf));
		tmp = strtok(buf, ",");
		length = 0;
		while(tmp != NULL)
		{
		    tmpNode->vals[length] = tmp;
		    length++;
		    tmp = strtok(NULL, ",");
		}
		strcpy(tmpNode->fileName, cmdFileName);
		tmpNode->length = length;
		tmpNode->prev = prev;
		tmpNode->next = NULL;
		prev->next = tmpNode;
		prev = tmpNode;
	    }
	    prev = NULL;
	    return head;
	}	
	else
	{
	    return NULL;
	}
	Close(fd);
    }
    else
	return table;
}

int getFieldLoc(int length, char **tableFields, char *fieldName)
{
    int count = 0;
    int i;
    for(i = 0; i < length; i++)
    {
	if(strcmp(tableFields[i], fieldName) == 0)
	    return i;
    }
    return -1;
}

tableNode **findNode(tableNode *table, char *val, char *fieldName)
{
    int fieldLoc = getFieldLoc(table->length, table->vals, fieldName);
    if(fieldLoc == -1)
	return NULL;

    tableNode *find = table->next;
    tableNode **findList = (tableNode **)malloc(sizeof(tableNode *)*30);
    int count = 0;
    // Find where field="val" using the location from the head array
    while(find != NULL)
    {
	if(strcmp(find->vals[fieldLoc], val) == 0)
	{
		findList[count] = find;
		count++;
	}
	find = find->next;
    }

    
    tableNode **new_findList;
    if(count == 0)
    {
	new_findList = (tableNode **)realloc(findList, sizeof(tableNode *)*1);
	new_findList[0] = NULL;
	printf("That field and value don't match a node");
    }
    else
	new_findList = (tableNode **)realloc(findList, sizeof(tableNode *)*count);
	
    return new_findList;
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

void freeTable(tableNode *node)
{
    while(node != NULL)
    {
	tableNode *tempTable = node->next;
	free(node);
	node = tempTable;
    }
}








