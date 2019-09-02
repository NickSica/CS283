#include "game.h"

void parentGame(int writefd[num_games][2], int readfd[num_games][2], char boards[num_games][dimensions][dimensions])
{
    // Parent is blue in all games
    // readfd should only use 0
    // writefd should only use 1
    int row, col;
    uint8_t *isDone = calloc(num_games, sizeof(uint8_t));
    uint8_t allIsDone = 0;
    rio_t rio[num_games];
    char move[MAX_LEN];
    int moveSize;
    for(int i = 0; i < num_games; i++)
    {
	// Read random first move
	Rio_readinitb(&(rio[i]), readfd[i][0]);
	read_move(rio[i], &row, &col);
	boards[i][row][col] = 'R';
	print_board(boards[i], i);

	// Make random first move
	srand(time(0)+time(0));
	int firstMove = rand() % dimensions;
	if(boards[i][0][firstMove] == 'R')
	    firstMove++;
	col = firstMove;
	row = 0;
	boards[i][0][firstMove] = 'B';
	print_board(boards[i], i);
	moveSize = sprintf(move, "%d %d\n", 0, firstMove);
	write(writefd[i][1], move, moveSize);
    }
   
    while(allIsDone < num_games)
    {
	for(int i = 0; i < num_games; i++)
	{
	    if(isDone[i] != 1)
	    {
		// Read move
		int check, enemy_row, enemy_col;
		read_move(rio[i], &enemy_row, &enemy_col);
		boards[i][enemy_row][enemy_col] = 'R';
		print_board(boards[i], i);
		check = check_win(boards[i], 'R');
		if(check)
		{
		    isDone[i] = 1;
		    allIsDone++;
		    printf("Game %d: Winner is red!\n", i);
		    Close(writefd[i][0]);
		    Close(writefd[i][1]);
		    Close(readfd[i][0]);
		    Close(readfd[i][1]);
		    continue;
		}

		// Make move
		boards[i][row][col] = 'B';
		moveSize = sprintf(move, "%d %d\n", row, col);
		write(writefd[i][1], move, moveSize);
		row++;
		print_board(boards[i], i);
		check = check_win(boards[i], 'B');
		if(check)
		{
		    isDone[i] = 1;
		    allIsDone++;
		    printf("Game %d: Winner is blue!\n", i);
		    Close(writefd[i][0]);
		    Close(writefd[i][1]);
		    Close(readfd[i][0]);
		    Close(readfd[i][1]);
		    continue;
		}
	    }
	}
    }
}

void childGame(int writefd, int readfd, char board[dimensions][dimensions])
{
    // Child is red
    char move[MAX_LEN];
    uint8_t isDone = 0;
    rio_t rio;
    Rio_readinitb(&rio, readfd);

    // Make random first move
    srand(time(0));
    int firstMove = rand() % dimensions;
    if(board[0][firstMove] == 'B')
    {
	if(firstMove != dimensions - 1)
	    firstMove++;
	else
	    firstMove--;
    }
    board[0][firstMove] = 'R';
    int moveSize = sprintf(move, "%d %d\n", 0, firstMove);
    write(writefd, move, moveSize);
    int col = firstMove;
    int row = 0;
    
    while(isDone == 0)
    {
	// Read move
	int enemy_row, enemy_col;
	read_move(rio, &enemy_row, &enemy_col);
	board[enemy_row][enemy_col] = 'B';
	isDone = check_win(board, 'B');
	if(isDone)
	    break;

	// Make move
	board[row][col] = 'R';
	row++;
	moveSize = sprintf(move, "%d %d\n", row, col);
	write(writefd, move, moveSize);
	isDone = check_win(board, 'R');
	if(isDone)
	    break;
    }
    Close(writefd);
    Close(readfd);
    exit(0);
}

void print_board(char board[dimensions][dimensions], int i)
{
    printf("Parent vs Child %d\n", i);
    for(int i = dimensions - 1; i >= 0; i--)
    {
	for(int j = 0; j < dimensions; j++)
	    printf("%c ", board[i][j]);
	printf("\n");
    }
    printf("\n");
}

void read_move(rio_t rio, int *row, int *col)
{
    // Read move
    char move[MAX_LEN];
    Rio_readlineb(&rio, move, MAX_LEN);
    *row = atoi(strtok(move, " "));
    *col = atoi(strtok(NULL, "\n"));
}

int check_win(char board[dimensions][dimensions], char color)
{
    int count = 0;
    for(int i = 0; i < dimensions; i++)
	for(int j = 0; j <= 3; j++)
	    if(board[0][i + j] == color)
		count++;
    if(count == 4)
	return 1;
    return 0;
}





