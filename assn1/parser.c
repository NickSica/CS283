#include "parser.h"

void parseSQL(char *input, command *cmd)
{
    char *cmdPart = strtok(input, " ");
    strcpy(cmd->operation, cmdPart);

    if(strcmp(cmdPart, "SELECT") == 0)
    {
        strtok(NULL, " ");
	strtok(NULL, " ");
	strcpy(cmd->fileNames[0], strtok(NULL, " "));
	cmd->longestFile = strlen(cmd->fileNames[0]);
	cmdPart = strtok(NULL, " ");
	strcpy(cmd->operation, strcat(strcat(cmd->operation, " "), cmdPart));
	if(strcmp(cmdPart, "WHERE") == 0)                                    // SELECT * FROM TableName WHERE Field1="value"
	{
	    strcpy(cmd->fieldNames[0], strtok(NULL, "=\""));
	    cmd->longestField = strlen(cmd->fieldNames[0]);
	    strcpy(cmd->vals[0], strtok(NULL, "\""));
	    cmd->longestVal = strlen(cmd->vals[0]);
	}
	else if(strcmp(cmdPart, "JOIN") == 0)                               // SELECT * FROM TableName JOIN TableName2 ON TableName.Field1 = TableName.Field2
	{
	    strcpy(cmd->fileNames[1], strtok(NULL, " "));
	    strtok(NULL, " ");
	    strcpy(cmd->fileNames[2], strtok(NULL, "."));
	    strcpy(cmd->fieldNames[0], strtok(NULL, "="));
	    strcpy(cmd->fileNames[3], strtok(NULL, "."));
	    strcpy(cmd->fieldNames[1], strtok(NULL, "\n"));

	    if(strlen(cmd->fieldNames[0]) > strlen(cmd->fieldNames[1]))
		cmd->longestField = strlen(cmd->fieldNames[0]);
	    else
		cmd->longestField = strlen(cmd->fieldNames[1]);

	    for(int i = 1; i < 4; i++)
		if(strlen(cmd->fileNames[i]) > cmd->longestFile)
		    cmd->longestFile = strlen(cmd->fileNames[i]);
	}
    }
    else if(strcmp(cmdPart, "UPDATE") == 0)                            // UPDATE TableName SET Field1="new value" WHERE Field2="value"
    {
	strcpy(cmd->fileNames[0], strtok(NULL, " "));
	strtok(NULL, " ");
	strcpy(cmd->fieldNames[0], strtok(NULL, "=\""));
	strcpy(cmd->vals[0], strtok(NULL, "\" "));
	strtok(NULL, " ");
	strcpy(cmd->fieldNames[1], strtok(NULL, "=\""));
	strcpy(cmd->vals[1], strtok(NULL, "\""));
	if(strlen(cmd->fieldNames[0]) > strlen(cmd->fieldNames[1]))
	    cmd->longestField = strlen(cmd->fieldNames[0]);
	else
	    cmd->longestField = strlen(cmd->fieldNames[1]);

	if(strlen(cmd->vals[0]) > strlen(cmd->vals[1]))
	    cmd->longestField = strlen(cmd->vals[0]);
	else
	    cmd->longestField = strlen(cmd->vals[1]);
    }

    else if(strcmp(cmdPart, "DROP") == 0)                                          // DROP TABLE TableName
    {
	strtok(NULL, " ");
	cmd->fileNames[0] = strtok(NULL, "\n");
    }
    else if(strcmp(cmdPart, "DELETE") == 0 || strcmp(cmdPart, "INSERT") == 0 || strcmp(cmdPart, "CREATE") == 0)
    {
	strtok(NULL, " ");
//        printf("This is %s\n", strtok(NULL, " "));
	strcpy(cmd->fileNames[0], strtok(NULL, " "));
	cmd->longestFile = strlen(cmd->fileNames[0]);
	
	if(strcmp(cmdPart, "DELETE") == 0)                                          // DELETE FROM TableName WHERE Field1="value"
	{
	    strtok(NULL, " ");
	    strcpy(cmd->fieldNames[0], strtok(NULL, "=\""));
	    strcpy(cmd->vals[0], strtok(NULL, "\""));
	}
	else if(strcmp(cmdPart, "INSERT") == 0)                                     // INSERT INTO TableName (Field1="value", Field2="value", ...)
	{
//	    printf("This is meh3: %s\n", meh3);
	    char **vals = malloc(sizeof(input));
	    char **finalFields = malloc(sizeof(input));
	    
	    char *fields = strtok(NULL, ")");
	    while(fields[0] == ' ' || fields[0] == '(')
		fields = fields + 1;
	    char *field = strtok(fields, "=");
	    cmd->longestField = strlen(field);
	    cmd->longestVal = 0;

	    int i = 0;
	    while(field != NULL)
	    {
		char *finalField = malloc(strlen(field)+1);
		strcpy(finalField, field);
		finalFields[i] = finalField;
		
		char *val = strtok(NULL, "\"");
		char *finalVal = malloc(strlen(val)+1);
		strcpy(finalVal, val);
		vals[i] = finalVal;
		
		if(strlen(field) > cmd->longestField)
		    cmd->longestField = strlen(field);
		if(strlen(val) > cmd->longestVal)
		    cmd->longestVal = strlen(val);
		field = strtok(NULL, " ");
		field = strtok(NULL, "=");
		i++;
	    }
	    printf("Done\n");
	    char **new_vals = realloc(vals, sizeof(char *)*i);
	    char **new_fields = realloc(finalFields, sizeof(char *)*i);
	    for(int j = 0; j < 2; j++)
	    {
		free(cmd->vals[j]);
		free(cmd->fieldNames[j]);
	    }
	    free(cmd->vals);
	    free(cmd->fieldNames);
	    cmd->vals = new_vals;
	    cmd->fieldNames = new_fields;
	    new_vals = NULL;
	    new_fields = NULL;
	    cmd->lengths = i;
	}
	else if(strcmp(cmdPart, "CREATE") == 0)                                   // CREATE TABLE TableName FIELDS [Field1, Field2, ...]
	{
	    strtok(NULL, "[");
	    char *fields = strtok(NULL, "]");
	    char *field = strtok(fields, ", ");
	    char **vals = malloc(sizeof(input));
	    cmd->longestVal = strlen(field);
	    
	    int i = 0;
	    while(field != NULL)
	    {
		char *val = malloc(sizeof(field));
		strcpy(val, field);
		vals[i] = val;
		if(strlen(field) > cmd->longestVal)
		    cmd->longestVal = strlen(field);
		field = strtok(NULL, ", ");
		i++;
	    }
	    char **new_vals = realloc(vals, sizeof(char *)*i);
	    for(int j = 0; j < 2; j++)
		free(cmd->vals[j]);
	    free(cmd->vals);
	    cmd->vals = new_vals;
	    new_vals = NULL;
	    cmd->lengths = i;
	}
    }
}















