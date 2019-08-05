#include "parser.h"

void parseSQL(char *input, command *cmd)
{
    char *cmdPart = strtok(input, " ");
    *(cmd->operation) = *cmdPart;
    if(strcmp(cmdPart, "SELECT") == 0)
    {
        strtok(NULL, " ");
	strtok(NULL, " ");
	cmd->fileNames[0] = strtok(NULL, " ");

	cmdPart = strtok(NULL, " ");
	cmd->operation = strcat(strcat(cmd->operation, " "), cmdPart);
	if(strcmp(cmdPart, "WHERE"))                                    // SELECT * FROM TableName WHERE Field1="value"
	{
	    cmd->fieldNames[0] = strtok(NULL, "=\"");
	    cmd->vals[0] = strtok(NULL, "\"");
	}
	else if(strcmp(cmdPart, "JOIN"))                               // SELECT * FROM TableName JOIN TableName2 ON TableName.Field1 = TableName.Field2
	{
	    cmd->fileNames[1] = strtok(NULL, " ");
	    strtok(NULL, " ");
	    cmd->fileNames[2] = strtok(NULL, ".");
	    cmd->fieldNames[0] = strtok(NULL, " = ");
	    cmd->fileNames[3] = strtok(NULL, ".");
	    cmd->fieldNames[1] = strtok(NULL, "\n");
	}
    }
    else if(strcmp(cmdPart, "UPDATE") == 0)                            // UPDATE TableName SET Field1="new value" WHERE Field2="value"
    {
	cmd->fileNames[0] = strtok(NULL, " ");
	strtok(NULL, " ");
	cmd->fieldNames[0] = strtok(NULL, "=\"");
	cmd->vals[0] = strtok(NULL, "\" ");
	strtok(NULL, " ");
	cmd->fieldNames[1] = strtok(NULL, "=\"");
	cmd->vals[1] = strtok(NULL, "\"");
    }
    else if(strcmp(cmdPart, "DROP") == 0)                                          // DROP TABLE TableName
    {
	strtok(NULL, " ");
	cmd->fileNames[0] = strtok(NULL, "\n");
    }
    else if(strcmp(cmdPart, "DELETE") == 0 || strcmp(cmdPart, "INSERT") == 0 || strcmp(cmdPart, "CREATE") == 0)
    {
        strtok(NULL, " ");
	if(strcmp(cmdPart, "DELETE") == 0 || strcmp(cmdPart, "CREATE") == 0)
	    strtok(NULL, " ");
	cmd->fileNames[0] = strtok(NULL, " ");
	
	if(strcmp(cmdPart, "DELETE") == 0)                                          // DELETE FROM TableName WHERE Field1="value"
	{
	    strtok(NULL, " ");
	    cmd->fieldNames[0] = strtok(NULL, "=\"");
	    cmd->vals[0] = strtok(NULL, "\"");
	}
	else if(strcmp(cmdPart, "INSERT") == 0)                                     // INSERT INTO TableName (Field1="value", Field2="value", ...)
	{
	    strtok(NULL, "(");
	    char *fields = strtok(NULL, ")");
	    char *field = strtok(fields, "=\"");
	    while(field != NULL)
	    {
		fieldNames = strcat(strcat(fieldNames, field), "\n");
		vals = strcat(strcat(vals, strtok(NULL, "\", ")), "\n");
	    }
	}
	else if(strcmp(cmdPart, "CREATE") == 0)                                   // CREATE TABLE TableName FIELDS [Field1, Field2, ...]
	{
	    strtok(NULL, "[");
	    char *fields = strtok(NULL, "]");
	    char *field = strtok(fields, ", ");
	    while(field != NULL)
	    {
		strcat(strcat(fieldNames, field), "\n");
		field = strtok(NULL, ", ");
	    }
	}
	
    }

}













