/*
 * Game2048.c
 *
 *  Created on: Jun 13, 2025
 *      Author: nthea
 */



#include "Game2048.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define BOARD_SIZE 4

int gameBoard[BOARD_SIZE][BOARD_SIZE] = {0};

extern void updateGameView(int board[BOARD_SIZE][BOARD_SIZE]);

static int fakeRandCounter = 0;
int fakeRand()
{
    fakeRandCounter++;
    return fakeRandCounter % 10;
}

void initGame(void)
{
	// Debug
	//printf("InitGame started\n");

	//Clear board
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			gameBoard[i][j] = 0;

	spawnRandomTile(gameBoard);
	spawnRandomTile(gameBoard);
	updateGameView(gameBoard);
}

void handleInputDirection(int direction)
{
	bool moved = false;

	switch(direction)
	{
		case 0: moved = moveLeft(gameBoard); break;
		case 1: moved = moveRight(gameBoard); break;
		case 2: moved = moveUp(gameBoard); break;
		case 3: moved = moveDown(gameBoard); break;
	}

	if (moved)
	{
		spawnRandomTile(gameBoard);
		updateGameView(gameBoard);
	}
}

static bool compressAndMergeRowLeft(int row[BOARD_SIZE])
{
	bool moved = false;
	int temp[BOARD_SIZE] = {0};
	int idx = 0;

	// Step 1: Shift to the left
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (row[i] != 0)
		{
			temp[idx++] = row[i];
		}
	}

	// Step 2: Merge the same numbers
	for (int i = 0; i < BOARD_SIZE - 1; i++)
	{
		if (temp[i] != 0 && temp[i] == temp [i + 1])
		{
			temp[i] *= 2;
			temp[i + 1] = 0;
			moved = true;
		}
	}

	// Step 3: Shift again after merging
	int final[BOARD_SIZE] = {0};
	idx = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (temp[i] != 0)
			final[idx++] = temp[i];
	}

	// Step 4: Check if any changes were made
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (row[i] != final[i])
		{
			moved = true;
			row[i] = final[i];
		}
	}

	return moved;
}

bool moveLeft(int board[BOARD_SIZE][BOARD_SIZE])
{
	bool moved = false;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (compressAndMergeRowLeft(board[i]))
			moved = true;
	}

	return moved;
}

bool moveRight(int board[BOARD_SIZE][BOARD_SIZE])
{
	bool moved = false;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int reversed[BOARD_SIZE];

		for (int j = 0; j < BOARD_SIZE; j++)
			reversed[j] = board[i][(BOARD_SIZE-1)-j];

		if (compressAndMergeRowLeft(board[i]))
					moved = true;

		for (int j = 0; j < BOARD_SIZE; j++)
			board[i][(BOARD_SIZE - 1)-j] = reversed[j];

	}

	return moved;
}

bool moveUp(int board[BOARD_SIZE][BOARD_SIZE])
{
	bool moved = false;

	for (int col = 0; col < BOARD_SIZE; col++)
	{
		int column[BOARD_SIZE];
		for (int row = 0; row < BOARD_SIZE; row++)
			column[row] = board[row][col];

		if (compressAndMergeRowLeft(column))
			moved = true;

		for (int row = 0; row < BOARD_SIZE; row++)
			board[row][col] = column[row];
	}

	return moved;
}

bool moveDown(int board[BOARD_SIZE][BOARD_SIZE])
{
	bool moved = false;

	for (int col = 0; col < BOARD_SIZE; col++)
	{
		int column[BOARD_SIZE];
		for (int row = 0; row < BOARD_SIZE; row++)
			column[row] = board[(BOARD_SIZE - 1) - row][col];

		if (compressAndMergeRowLeft(column))
			moved = true;

		for (int row = 0; row < BOARD_SIZE; row++)
			board[(BOARD_SIZE - 1) - row][col] = column[row];
	}

	return moved;
}

void spawnRandomTile(int board[BOARD_SIZE][BOARD_SIZE])
{
	int emptyCells[BOARD_SIZE*BOARD_SIZE][2];
	int count = 0;

	// Traverse the entire board to find empty cells
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == 0)
			{
				emptyCells[count][0] = i;
				emptyCells[count][1] = j;
				count++;
			}
		}

	// If there are empty cells, randomly select one
	if (count > 0)
	{
		// Index of the randomly chosen empty cell
		int idx = fakeRand() % count;

		int i = emptyCells[idx][0];
		int j = emptyCells[idx][1];

		// Assign the value 2 or 4 (90% chance of 2)
		board[i][j] = (fakeRand() < 9) ? 2 : 4;
		printf("%d", board[i][j]);
	}
}

