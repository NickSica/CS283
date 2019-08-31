#include "game.h"

void parentGame(int writefd[num_games][2], int readfd[num_games][2], char boards[num_games][dimensions][dimensions])
{
    // Parent is blue in all games
    // readfd should only use 0
    // writefd should only use 1
    uint8_t isDone = 0;
    rio_t rio[num_games];
    for(int i = 0; i < num_games; i++)
    {
	pid_t pid;
	if(num_games != 1)
	{
	    pid = fork();
	    if(pid != 0 && i != num_games - 1)
		continue;
	}
	
	rio_readinitb(&rio[i], readfd[i][0]);
	while(isDone == 0)
	{
	    int row, col;
	    read_move(&rio[i], &row, &col);
	    boards[i][row][col] = 'R';
	    isDone = check_win(boards[i], 'R');
	    
	    make_move(writefd[i][1], boards[i], 'B', &row, &col);
	    boards[i][row][col] = 'B';
	    isDone = check_win(boards[i], 'B');
	}
    }
}

void childGame(int writefd, int readfd, char board[dimensions][dimensions])
{
    // Child is red
    char move[MAX_LEN];
    uint8_t isDone = 0;
    rio_t rio;
    rio_readinitb(&rio, readfd);

    int firstMove = dimensions / 2;
    board[firstMove][firstMove] = 'R';
    int moveSize = sprintf(move, "%d %d\n", firstMove, firstMove);
    write(writefd, move, moveSize);
    
    while(isDone == 0)
    {
	int row, col;
	read_move(&rio, &row, &col);
	board[row][col] = 'B';
	isDone = check_win(board, 'B');
	
	make_move(writefd, board, 'R', &row, &col);
	board[row][col] = 'R';
	isDone = check_win(board, 'R');
    }
}

void read_move(rio_t *rio, int *row, int *col)
{
    // Read move
    char move[MAX_LEN];
    rio_readlineb(rio, move, MAX_LEN);
    *row = (int)strtok(move, " ");
    *col = (int)strtok(NULL, "\n");
}

void make_move(int writefd, char board[dimensions][dimensions], char color, int *row, int *col)
{
    // Analyze board and return optimal move
    char move[MAX_LEN];
    int opt_move[2];
    // TODO: Add board analysis
    // Check to see if he is about to win
    
    // Check for up/down

    // Check for diagonal left

    // Check for diagonal right

    // Will he win if this move is made??

    *row = opt_move[0];
    *col = opt_move[1];
    
    // Send move
    int moveSize = sprintf(move, "%d %d\n", opt_move[0], opt_move[1]);
    write(writefd, move, moveSize);
}

int check_win(char board[dimensions][dimensions], char color)
{
    return 0;
}











