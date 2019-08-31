#ifndef __GAME_H__
#define __GAME_H__

#define MAX_LEN 100

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "csapp.h"

int num_games;
int dimensions;

void parentGame(int writefd[num_games][2], int readfd[num_games][2], char boards[num_games][dimensions][dimensions]);
void childGame(int writefd, int readfd, char board[dimensions][dimensions]);
void read_move(rio_t *rio, int *row, int *col);
void make_move(int writefd, char board[dimensions][dimensions], char color, int *row, int *col);
int check_win(char board[dimensions][dimensions], char color);
#endif
