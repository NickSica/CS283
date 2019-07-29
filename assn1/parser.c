#include "parser.h"

void parseSQL(char *input, char *cmd, char *fileName, char *vals)
{
    char *cmdPart = strtok(input, " ");
    *vals = "";
    if(strcmp(cmdPart, "SELECT") == 0)
        *cmd = *cmdPart + strtok(NULL, " ") + strtok(NULL, " ");
    else if(strcmp(cmdPart, "UPDATE") == 0)
        *cmd = *cmdPart;
    else if(strcmp(cmdPart, "DELETE") == 0 || strcmp(cmdPart, "INTO") == 0 || strcmp(cmdPart, "CREATE") == 0 || strcmp(cmdPart, "DROP") == 0)
        *cmd = *cmdPart + strtok(NULL, " ");

    *fileName = strtok(NULL, " ");

/*
SELECT * FROM    TableName WHERE  Field1="value"
SELECT * FROM    TableName JOIN   TableName2 ON TableName.Field1 = TableName.Field2
UPDATE           TableName SET    Field1="new value" WHERE Field2="value"
DELETE FROM      TableName WHERE  Field1="value"
INSERT INTO      TableName        (Field1="value", Field2="value", ...)
CREATE TABLE     TableName FIELDS [Field1, Field2, ...]
DROP TABLE       TableName
*/
}