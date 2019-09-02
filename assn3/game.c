#include "game.h"

void parentGame(int writefd[num_games][2], int readfd[num_games][2], char boards[num_games][dimensions][dimensions])
{
    // Parent is blue in all games
    // readfd should only use 0
    // writefd should only use 1
    uint8_t *isDone = calloc(num_games, sizeof(uint8_t));
    uint8_t allIsDone = 0;
    rio_t rio[num_games];
    for(int i = 0; i < num_games; i++)
    {
	int row, col;
	read_move(&rio[i], &row, &col);
	boards[i][row][col] = 'R';
	srand(time(0));
	int firstMove = rand() % dimensions;
	
	if(boards[i][0][firstMove] == 'R')
	    boards[i][1][firstMove] = 'B';
	else
	    boards[i][0][firstMove] = 'B';

	char move[MAX_LEN];
	int moveSize = sprintf(move, "%d %d\n", firstMove, firstMove);
	write(writefd[i][1], move, moveSize);	
	rio_readinitb(&rio[i], readfd[i][0]);
    }
    
    while(allIsDone < num_games)
    {
	for(int i = 0; i < num_games; i++)
	{
	    if(isDone[i] != 1)
	    {
		int row, col;
		int check;
		read_move(&rio[i], &row, &col);
		boards[i][row][col] = 'R';
		check = check_win(boards[i], 'R');
		if(check)
		{
		    isDone[i] = 1;
		    allIsDone++;
		    continue;
		}
		
		make_move(writefd[i][1], boards[i], 'B', &row, &col);
		boards[i][row][col] = 'B';
		check = check_win(boards[i], 'B');
		if(check)
		{
		    isDone[i] = 1;
		    allIsDone++;
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
    char winner;
    rio_t rio;
    rio_readinitb(&rio, readfd);
    
    srand(time(0));
    int firstMove = rand() % dimensions;    
    board[0][firstMove] = 'R';
    int moveSize = sprintf(move, "%d %d\n", firstMove, firstMove);
    write(writefd, move, moveSize);
    
    while(isDone == 0)
    {
	int row, col;
	read_move(&rio, &row, &col);
	board[row][col] = 'B';
	isDone = check_win(board, 'B');
	if(isDone)
	{
	    winner = 'B';
	    break;
	}
	print_board(board);
	
	make_move(writefd, board, 'R', &row, &col);
	board[row][col] = 'R';
	isDone = check_win(board, 'R');
	if(isDone)
	{
	    winner = 'R';
	    break;
	}
    }
}

void print_board(char board[dimensions][dimensions])
{
    for(int i = 0; i < dimensions; i++)
    {
	for(int j = 0; j < dimensions; j++)
	    printf("%c ", board[i][j]);
	printf("\n");
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
    for(int i = 0; i < dimensions; i++)
    {
	for(int j = 0; j < dimensions; j++)
	{
	    if(board[i][j] == color)
	    {
		uint8_t is_diagonal_right_up = 0b100;
		uint8_t is_diagonal_left_up = 0b100;
		uint8_t is_diagonal_right_down = 0b100;
		uint8_t is_diagonal_left_down = 0b100;
		uint8_t is_horizontal = 0b100;
		uint8_t is_vertical = 0b100;
		for(int h = 1; h <= 3; h++)
		{
		    // The first statement stores the count in the first 2 bits
		    // The second statement stores the location in the bits
		    // It become 0 if the path was blocked in previous iterations
		    if(i + h < dimensions && board[i + h][j] == 'R' && board[i + h][j] == 'B')
		    {
			if(is_vertical != 0 && board[i + h][j] == color)
			{
			    is_vertical++;
			    is_vertical = is_vertical | (0b1 << 2 << i);
			}
			else if(is_vertical != 0)
			{
			    is_vertical = 0;
			}
		    }

		    if(j + h < dimensions && board[i][j + h] == 'R' && board[i][j + h] == 'B')
		    {
			if(is_horizontal != 0 && board[i][j + h] == color)
			{
			    is_horizontal++;
			    is_horizontal = is_horizontal | (0b1 << 2 << i);
			}
			else if(is_horizontal != 0)
			{
			    is_horizontal = 0;
			}			
		    }
		    
		    if(i + h < dimensions && j + h < dimensions && board[i + h][j + h] == 'R' && board[i + h][j + h] == 'B')
		    {
			if(is_diagonal_right_up != 0 && board[i + h][j + h] == color)
			{
			    is_diagonal_right_up++;
			    is_diagonal_right_up = is_diagonal_right_up | (0b1 << 2 << i);
			}
			else if(is_diagonal_right_up != 0)
			{
			    is_diagonal_right_up = 0;
			}			
		    }
		    
		    if(i - h < dimensions && j + h < dimensions && board[i - h][j + h] == 'R' && board[i - h][j + h] == 'B')
		    {
			if(is_diagonal_right_down != 0 && board[i - h][j + h] == color)
			{
			    is_diagonal_right_down++;
			    is_diagonal_right_down = is_diagonal_right_down | (0b1 << 2 << i);
			}
			else if(is_diagonal_right_down != 0)
			{
			    is_diagonal_right_down = 0;
			}			
		    }
		    
		    if(i + h < dimensions && j - h < dimensions && board[i + h][j - h] == 'R' && board[i + h][j - h] == 'B')
		    {
			if(is_diagonal_left_up != 0 && board[i + h][j - h] == color)
			{
			    is_diagonal_left_up++;
			    is_diagonal_left_up = is_diagonal_left_up | (0b1 << 2 << i);
			}
			else if(is_diagonal_left_up != 0)
			{
			    is_diagonal_left_up = 0;
			}			
		    }
		    
		    if(i - h < dimensions && j - h < dimensions && board[i - h][j - h] == 'R' && board[i - h][j - h] == 'B')
		    {
			if(is_diagonal_left_down != 0 && board[i - h][j - h] == color)
			{
			    is_diagonal_left_down++;
			    is_diagonal_left_down = is_diagonal_left_down | (0b1 << 2 << i);
			}
			else if(is_diagonal_left_down != 0)
			{
			    is_diagonal_left_down = 0;
			}			
		    }
		}
		
		uint8_t max = (is_vertical & 0b11);
		*row = 0;
		*col = 0;
		for(int h = 1; h < 3; h++)
		{
		    uint8_t bit_pos = (is_vertical & (0b1111 << 2)) >> 2;
		    if(((bit_pos & (1 << i)) >> i) == 0)
		    {
			*row = i + h;
			*col = j;
			max = i;
		    }
		}

		if(max < (is_horizontal & 0b11))
		{
		    uint8_t bit_pos = (is_horizontal & (0b1111 << 2)) >> 2;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << i)) >> i) == 0)
			{
			    if(board[i - 1][j + h] != 'E')
			    {
				*row = i;
				*col = j + h;
				max = (is_horizontal & 0b11);
			    }
			}
		    }
		}

		if(max < (is_diagonal_right_up & 0b11))
		{
		    uint8_t bit_pos = (is_diagonal_right_up & (0b1111 << 2)) >> 2;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << i)) >> i) == 0)
			{
			    if(board[i + h - 1][j + h] != 'E')
			    {
				*row = i + h;
				*col = j + h;
				max = (is_diagonal_right_up & 0b11);
			    }
			}
		    }
		}

		if(max < (is_diagonal_right_down & 0b11))
		{
		    uint8_t bit_pos = (is_diagonal_right_down & (0b1111 << 2)) >> 2;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << i)) >> i) == 0)
			{
			    if(board[i - h - 1][j + h] != 'E')
			    {
				*row = i - h;
				*col = j + h;
				max = (is_diagonal_right_down & 0b11);
			    }
			}
		    }
		}
		
		if(max < (is_diagonal_left_up & 0b11))
		{
		    uint8_t bit_pos = (is_diagonal_left_up & (0b1111 << 2)) >> 2;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << i)) >> i) == 0)
			{
			    if(board[i + h - 1][j - h] != 'E')
			    {
				*row = i + h;
				*col = j - h;
				max = (is_diagonal_left_up & 0b11);
			    }
			}
		    }
		}

		if(max < (is_diagonal_left_down & 0b11))
		{
		    uint8_t bit_pos = (is_diagonal_left_down & (0b1111 << 2)) >> 2;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << i)) >> i) == 0)
			{
			    if(board[i - h - 1][j - h] != 'E')
			    {
				*row = i - h;
				*col = j - h;
				max = (is_diagonal_left_down & 0b11);
			    }
			}
		    }
		}
	    }
	}
    }

    // Send move
    int moveSize = sprintf(move, "%d %d\n", *row, *row);
    write(writefd, move, moveSize);
}

int check_win(char board[dimensions][dimensions], char color)
{
    for(int i = 0; i < dimensions; i++)
    {
	for(int j = 0; j < dimensions; j++)
	{
	    if(board[i][j] == color)
	    {
		uint8_t count_vertical = 1;
		uint8_t count_horizontal = 1;
		uint8_t count_diagonal_right_up = 1;
		uint8_t count_diagonal_right_down = 1;
		uint8_t count_diagonal_left_up = 1;
		uint8_t count_diagonal_left_down = 1;
		for(int h = 1; h < 3; h++)
		{
		    if(board[i + h][j] == color)
			count_vertical++;
		    if(board[i][j + h] == color)
			count_horizontal++;
		    if(board[i + h][j + h] == color)
			count_diagonal_right_up++;
		    if(board[i - h][j + h] == color)
			count_diagonal_right_down++;
		    if(board[i + h][j - h] == color)
			count_diagonal_left_up++;
		    if(board[i - h][j - h] == color)
			count_diagonal_left_down++;
		}
		if(count_vertical == 4 || count_horizontal == 4 || count_diagonal_right_up == 4 || count_diagonal_right_down == 4 ||
		   count_diagonal_left_up == 4 || count_diagonal_left_down == 4)
		    return 1;
	    }
	}
    }

    return 0;
}











