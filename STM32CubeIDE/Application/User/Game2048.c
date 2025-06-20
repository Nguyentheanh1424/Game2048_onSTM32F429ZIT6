/*
 * Game2048.c
 *
 *  Created on: Jun 13, 2025
 *      Author: nthea
 */

#include "Game2048.h"
#include "main.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#define BOARD_SIZE 4
#define BEST_SCORE_BKP_REG RTC_BKP_DR0

bool flagContinue = false;
int gameBoard[BOARD_SIZE][BOARD_SIZE] = {0};
int currentGameState = GAME_PLAYING;
int score = 0;

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;
extern volatile bool shouldUpdate;
extern void playAudio(int action);

// ========== LCG RANDOM IMPLEMENTATION ==========
static uint32_t randSeed = 1;

void initRandom(void)
{
    randSeed = HAL_GetTick();

    randSeed ^= (uint32_t)&randSeed;

    if (randSeed == 0) randSeed = 1;
}

uint32_t customRand(void)
{
    // Linear Congruential Generator (LCG)
    randSeed = randSeed * 1664525U + 1013904223U;
    return randSeed;
}

int betterRand(void)
{
    return (int)(customRand() % 10);
}

void initGame(void)
{
    score = 0;

    initRandom();

    // Clear board
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            gameBoard[i][j] = 0;

    currentGameState = GAME_PLAYING;

    spawnRandomTile(gameBoard);
    spawnRandomTile(gameBoard);
}

void handleInputDirection(int direction)
{
    // Don't allow moves if game is over
    if (currentGameState == GAME_OVER) {
        return;
    }

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
    	playAudio(2);
        spawnRandomTile(gameBoard);

        // Check win condition
        if (currentGameState == GAME_PLAYING && checkWin(gameBoard)) {
            currentGameState = GAME_WON;
        }

        // Check game over condition
        if (checkGameOver(gameBoard)) {
            currentGameState = GAME_OVER;
        }
    }
}

static bool compressAndMergeRowLeft(int row[BOARD_SIZE])
{
    bool moved = false;
    int last = 0, insertPos = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (row[i] == 0) continue;

        if (last == 0) {
            last = row[i];
        } else if (last == row[i]) {
            row[insertPos++] = last * 2;
            score += last * 2;
            last = 0;
            moved = true;
        } else {
            row[insertPos++] = last;
            last = row[i];
        }
        if (i != insertPos) moved = true;
    }

    if (last != 0) {
        row[insertPos++] = last;
    }

    // Fill remaining with 0
    while (insertPos < BOARD_SIZE) row[insertPos++] = 0;

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

        // Reverse the row
        for (int j = 0; j < BOARD_SIZE; j++)
            reversed[j] = board[i][(BOARD_SIZE-1)-j];

        // Process the reversed row (FIXED BUG)
        if (compressAndMergeRowLeft(reversed))
            moved = true;

        // Reverse back and assign to board
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
        // Index of the randomly chosen empty cell - SỬ DỤNG LCG
        int idx = (int)(customRand() % count);

        int i = emptyCells[idx][0];
        int j = emptyCells[idx][1];

        // Assign the value 2 or 4 (90% chance of 2, 10% chance of 4)
        uint32_t randValue = customRand() % 100;
        board[i][j] = (randValue < 90) ? 2 : 4;
        shouldUpdate = true;
    }
}

bool checkWin(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] >= WIN_TILE)
            {
                return (true & !flagContinue) ;
            }
        }
    }
    return false;
}

bool checkGameOver(int board[BOARD_SIZE][BOARD_SIZE])
{
    // If there are empty cells, game is not over
    if (hasEmptyCell(board)) {
        return false;
    }

    // If no empty cells but can still merge, game is not over
    if (canMerge(board)) {
        return false;
    }

    // No empty cells and no possible merges = game over
    return true;
}

bool hasEmptyCell(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] == 0)
                return true;
    return false;
}

bool canMerge(int board[BOARD_SIZE][BOARD_SIZE])
{
    // Check horizontal merges
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE - 1; j++)
            if (board[i][j] == board[i][j + 1] && board[i][j] != 0)
            	return true;

    // Check vertical merges
    for (int i = 0; i < BOARD_SIZE - 1; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] == board[i + 1][j] && board[i][j] != 0)
            	return true;

    return false;
}

void SaveBestScore(int score)
{
    HAL_RTCEx_BKUPWrite(&hrtc, BEST_SCORE_BKP_REG, (uint32_t)score);
}

int LoadBestScore(void)
{
    return (int)HAL_RTCEx_BKUPRead(&hrtc, BEST_SCORE_BKP_REG);
}


int (*getGameBoard(void))[BOARD_SIZE]
{
    return gameBoard;
}
