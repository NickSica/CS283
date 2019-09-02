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
    for(int i = 0; i < num_games; i++)
    {
	Rio_readinitb(&(rio[i]), readfd[i][0]);
	read_move(rio[i], &row, &col);
	boards[i][row][col] = 'R';
	print_board(boards[i], i);
	
	srand(time(0)+time(0));
	int firstMove = rand() % dimensions;
	if(boards[i][0][firstMove] == 'R')
	    firstMove++;
	col = firstMove;
	row = 0;
	boards[i][0][firstMove] = 'B';
	print_board(boards[i], i);
	
	char move[MAX_LEN];
	int moveSize = sprintf(move, "%d %d\n", 0, firstMove);
	write(writefd[i][1], move, moveSize);	
    }
   
    while(allIsDone < num_games)
    {
	for(int i = 0; i < num_games; i++)
	{
	    if(isDone[i] != 1)
	    {
		int check, enemy_row, enemy_col;
		read_move(rio[i], &enemy_row, &enemy_col);
		boards[i][enemy_row][enemy_col] = 'R';
		print_board(boards[i], i);
		check = check_win(boards[i], 'R');		
		if(check)
		{
		    isDone[i] = 1;
		    allIsDone++;
		    printf("Game %d: Winner is red!", i);
		    continue;
		}

		//make_move(writefd[i][1], boards[i], 'B', &row, &col);
		char move[MAX_LEN];
		boards[i][row][col] = 'B';
		int moveSize = sprintf(move, "%d %d\n", row, col);
		write(writefd[i][1], move, moveSize);
		row++;
		print_board(boards[i], i);
		check = check_win(boards[i], 'B');		
		if(check)
		{
		    isDone[i] = 1;
		    allIsDone++;
		    printf("Game %d: Winner is blue!", i);
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
    
    srand(time(0));
    int firstMove = rand() % dimensions;
    if(board[0][firstMove] == 'B')
	firstMove = firstMove + 1;
    board[0][firstMove] = 'R';
    int moveSize = sprintf(move, "%d %d\n", 0, firstMove);
    write(writefd, move, moveSize);
    int col = firstMove;
    int row = 0;
    
    while(isDone == 0)
    {
	int enemy_row, enemy_col;
	read_move(rio, &enemy_row, &enemy_col);
	board[enemy_row][enemy_col] = 'B';
	isDone = check_win(board, 'B');
	if(isDone)
	    break;

	//make_move(writefd, board, 'R', &row, &col);	
	board[row][col] = 'R';
	row++;
	moveSize = sprintf(move, "%d %d\n", row, col);
	write(writefd, move, moveSize);
	isDone = check_win(board, 'R');
	if(isDone)
	    break;
    }
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

void make_move(int writefd, char board[dimensions][dimensions], char color, int *row, int *col)
{
    // Analyze board and return optimal move
    char move[MAX_LEN];
    uint8_t max = 4;
    *row = 0;
    *col = 0;

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
		    is_vertical = check_direction(is_vertical, board, i + h, j, h, color);
		    is_horizontal = check_direction(is_horizontal, board, i, j + h, h, color);
		    is_diagonal_right_up = check_direction(is_diagonal_right_up, board, i + h, j + h, h, color);
		    is_diagonal_right_down = check_direction(is_diagonal_right_down, board, i - h, j + h, h, color);
		    is_diagonal_left_up = check_direction(is_diagonal_left_up, board, i + h, j - h, h, color);
		    is_diagonal_left_down = check_direction(is_diagonal_left_down, board, i - h, j - h, h, color);
		}
		
		if(max < (is_vertical & 0b11) || max == 4)
		{
		    uint8_t bit_pos = (is_vertical & (0b1111 << 2)) >> 2;
		    max = is_vertical & 0b11;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << h)) >> h) == 0)
			{
			    *row = i + h;
			    *col = j;
			    break;
			}
		    }
		}

		if(max < (is_horizontal & 0b11))
		{
		    uint8_t bit_pos = (is_horizontal & (0b1111 << 2)) >> 2;
		    max = is_horizontal & 0b11;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << h)) >> h) == 0)
			{
			    if(board[i - 1][j + h] != 'E')
			    {
				*row = i;
				*col = j + h;
				break;
			    }
			}
		    }
		}

		if(max < (is_diagonal_right_up & 0b11))
		{
		    uint8_t bit_pos = (is_diagonal_right_up & (0b1111 << 2)) >> 2;
		    max = is_diagonal_right_up & 0b11;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << h)) >> h) == 0)
			{
			    if(board[i + h - 1][j + h] != 'E')
			    {
				*row = i + h;
				*col = j + h;
				break;
			    }
			}
		    }
		}

		if(max < (is_diagonal_right_down & 0b11))
		{
		    uint8_t bit_pos = (is_diagonal_right_down & (0b1111 << 2)) >> 2;
		    max = is_diagonal_right_down & 0b11;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << h)) >> h) == 0)
			{
			    if(board[i - h - 1][j + h] != 'E')
			    {
				*row = i - h;
				*col = j + h;
				break;
			    }
			}
		    }
		}
		
		if(max < (is_diagonal_left_up & 0b11))
		{
		    uint8_t bit_pos = (is_diagonal_left_up & (0b1111 << 2)) >> 2;
		    max = is_diagonal_left_up & 0b11;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << h)) >> h) == 0)
			{
			    if(board[i + h - 1][j - h] != 'E')
			    {
				*row = i + h;
				*col = j - h;
				break;
			    }
			}
		    }
		}

		if(max < (is_diagonal_left_down & 0b11))
		{
		    uint8_t bit_pos = (is_diagonal_left_down & (0b1111 << 2)) >> 2;
		    max = is_diagonal_left_down & 0b11;
		    for(int h = 1; h < 3; h++)
		    {
			if(((bit_pos & (1 << h)) >> h) == 0)
			{
			    if(board[i - h - 1][j - h] != 'E')
			    {
				*row = i - h;
				*col = j - h;
				break;
			    }
			}
		    }
		}
	    }
	}
    }

    // Send move
    int moveSize = sprintf(move, "%d %d\n", *row, *col);
    write(writefd, move, moveSize);
}

uint8_t check_direction(uint8_t direction, char board[dimensions][dimensions], int i, int j, int h, char color)
{
    if(i < dimensions && board[i][j] == 'R' && board[i][j] == 'B')
    {
	if(direction != 0 && board[i][j] == color)
	{
	    direction++;
	    direction = direction | (0b1 << 2 << h);
	}
	else if(direction != 0)
	{
	    direction = 0;
	}
    }
    return direction;
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
