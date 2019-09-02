#include "game.h"

int main(int argc, char **argv)
{
    if(argc < 3)
    {
	num_games = 1;
	dimensions = 8;
    }

    num_games = atoi(argv[1]);
    dimensions = atoi(argv[2]);

    if(dimensions == 0 || num_games == 0)
    {
	printf("Incorrect amount of games or dimensions, using default.");
	num_games = 1;
	dimensions = 8;
    }
    

    char boards[num_games][dimensions][dimensions];
    int parent_to_child[num_games][2];
    int child_to_parent[num_games][2];
    pid_t pid[num_games];
    
    for(int i = 0; i < num_games; i++)
    {
	for(int j = 0; j < dimensions; j++)
	    boards[i][j][j] = 'E';
	
	if(pipe(parent_to_child[i]) < 0)
	    perror("Error: Pipe initialization failed");
	if(pipe(child_to_parent[i]) < 0)
	    perror("Error: Pipe initialization failed");

	int writefd[2];
	writefd[0] = child_to_parent[i][0];
	writefd[1] = child_to_parent[i][1];
	int readfd[2];
	readfd[0] = parent_to_child[i][0];
	readfd[1] = parent_to_child[i][1];

	close(parent_to_child[i][0]);
	close(child_to_parent[i][1]);
	close(writefd[0]);
	close(readfd[1]);
	
	pid[i] = fork();
	if(pid[i] == 0)
	    childGame(writefd[1], readfd[0], boards[i]);
    }
    parentGame(parent_to_child, child_to_parent, boards);

    
    return 0;
}







