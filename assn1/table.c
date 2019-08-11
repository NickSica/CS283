#include "table.h"

void writeTable(tableNode *table, char *dbDir)
{
    char *path = malloc(sizeof(dbDir)+50);
    char *tmpPath = malloc(sizeof(dbDir)+50);
    strcpy(path, dbDir);
    strcat(path, table->fileName);
    strcpy(tmpPath, path);
    strcat(tmpPath, ".tmp");
    
    int fd = Open(tmpPath, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    tableNode* currNode = table;
    while(currNode != NULL)
    {
	int i;
	for(i = 0; i < currNode->length; i++)
	{
	    char *buf = malloc(strlen(currNode->vals[i]));
	    strcpy(buf, currNode->vals[i]);
	    
	    if(i == currNode->length - 1)
		strcat(buf, "\n");
	    else
		strcat(buf, ",");
	    Rio_writen(fd, buf, strlen(buf));
	    free(buf);
	}
	currNode = currNode->next;
    }
    Close(fd);
    rename(tmpPath, path);
    free(path);
    free(tmpPath);
}

tableNode *openTable(tableNode *table, size_t valsSize, size_t oneValSize, char *cmdFileName, char *dbDir, char *op)
{
    if(strcmp(table->fileName, cmdFileName) != 0)
    {
	freeTable(table);
	char *path = malloc(strlen(dbDir)+50);
	strcpy(path, dbDir);
	strcat(path, cmdFileName);
	int fd = Open(path, O_CREAT | O_RDONLY, S_IRWXU | S_IRWXG);
	int n;
	rio_t rio;
	char buf[MAXLINE];
	
	Rio_readinitb(&rio, fd);
	if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0 && strcmp(op, "CREATE") != 0)
	{
	    int length = 0;
	    char *tmp = strtok(buf, ",");
	    char **vals = malloc(sizeof(char *)*strlen(buf));
	    unsigned long longestVal = 0;
	    while(tmp != NULL)
	    {
		char *val = malloc(sizeof(char)*(strlen(tmp)+1));
		strcpy(val, tmp);
		if(strlen(tmp) > longestVal)
		    longestVal = strlen(tmp);
		vals[length] = val;
		length++;
		tmp = strtok(NULL, ",\n");
	    }
	    char **new_vals = realloc(vals, sizeof(char*)*length);
	    table = allocTable(sizeof(char *)*length, sizeof(char)*longestVal, sizeof(cmdFileName));
	    for(int i = 0; i < length; i++)
		strcpy(table->vals[i], new_vals[i]);
	    
	    for(int i = 0; i < length; i++)
		free(new_vals[i]);
	    free(new_vals);
	    strcpy(table->fileName, cmdFileName);
	    table->next = NULL;
	    table->prev = NULL;
	    table->length = length;
	    tableNode *prev = table;
	
	    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
	    {
		tableNode *tmpNode = allocTable(sizeof(table->vals), sizeof(table->vals[0]), sizeof(table->fileName));
		tmp = strtok(buf, ",");
		length = 0;
		while(tmp != NULL)
		{
		    strcpy(tmpNode->vals[length], tmp);
		    length++;
		    tmp = strtok(NULL, ",\n");
		}
		strcpy(tmpNode->fileName, cmdFileName);
		tmpNode->length = length;
		tmpNode->prev = prev;
		tmpNode->next = NULL;
		prev->next = tmpNode;
		prev = tmpNode;
	    }
	    prev = NULL;
	}	
	else
	    table = allocTable(valsSize, oneValSize, sizeof(cmdFileName));
	Close(fd);
	free(path);
    }
    return table;
}

int getFieldLoc(int length, char **tableFields, char *fieldName)
{
    int i;
    for(i = 0; i < length; i++)
    {
	if(strcmp(tableFields[i], fieldName) == 0)
	    return i;
    }
    return -1;
}

tableNode **findNode(tableNode *table, char *val, char *fieldName, int *numNodes)
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
	new_findList = realloc(findList, sizeof(tableNode *)*1);
	new_findList[0] = NULL;
	printf("That field and value don't match a node");
    }
    else
	new_findList = realloc(findList, sizeof(tableNode *)*count);

    *numNodes = count;
    return new_findList;
}

void printNode(tableNode *node)
{
    if(node == NULL)
	printf("Value not found.\n");
    else
    {
	for(int i = 0; i < node->length-1; i++)
	    printf("%s,", node->vals[i]);
	printf("%s\n", node->vals[node->length-1]);
    }	       
}

tableNode *allocTable(size_t valsSize, size_t oneValSize, size_t fileNameSize)
{
    tableNode *node = malloc(sizeof(tableNode));
    node->fileName = malloc(fileNameSize);
    node->vals = malloc(sizeof(char *)*valsSize);
    for(int i = 0; i < (int)valsSize; i++)
	node->vals[i] = malloc(sizeof(char)*oneValSize);    
    return node;
}

void freeTable(tableNode *node)
{
    while(node != NULL)
    {
	tableNode *tempTable = node->next;    
	int i;
	for(i = 0; i < node->length; i++)
	{
	    free(node->vals[i]);
	}
	free(node->fileName);
	free(node->vals);
	free(node);
	node = tempTable;
    }
}











