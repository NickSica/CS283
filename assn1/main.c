#include <stdlib.h>

#include "parser.h"

int main(void)
{
    char **activeTable1;
    char **activeTable2;
    char *input;
    command *cmd = malloc(sizeof(input));
    parseSQL(input, cmd);

    if(strcmp(cmd, "SELECT WHERE") == 0)
    {
	activeTable1 = malloc(sizeof(char *));
	activeTable1->name = file1;
    }
    else if(strcmp(cmd, "SELECT JOIN") == 0)
    {

    }
    else if(strcmp(cmd, "DELETE") == 0)
    {

    }
    else if(strcmp(cmd, "UPDATE") == 0)
    {

    }
    else if(strcmp(cmd, "INSERT") == 0)
    {

    }
    else if(strcmp(cmd, "CREATE") == 0)
    {

    }
    else if(*cmd == "DROP")
    {
	
    }

    
    return 0;
}









