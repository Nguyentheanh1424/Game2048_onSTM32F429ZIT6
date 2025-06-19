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

int gameBoard[BOARD_SIZE][BOARD_SIZE] = {0};
int currentGameState = GAME_PLAYING;
int score = 0;

extern UART_HandleTypeDef huart1;
extern volatile bool shouldUpdate;

// ========== LCG RANDOM IMPLEMENTATION ==========
static uint32_t randSeed = 1;

void initRandom(void)
{
    randSeed = HAL_GetTick();

    randSeed ^= (uint32_t)&randSeed;

    if (randSeed == 0) randSeed = 1;

    debugPrintString("Random seed initialized: ");
    debugPrintNumber(randSeed);
    debugPrintString("\r\n");
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

// Debug functions for STM32 UART1
void debugPrintString(const char* str)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

void debugPrintNumber(int num)
{
    char buffer[16];
    sprintf(buffer, "%d", num);
    debugPrintString(buffer);
}

void debugPrintGameState(int state)
{
    switch(state)
    {
        case GAME_PLAYING:
            debugPrintString("State: PLAYING\r\n");
            break;
        case GAME_WON:
            debugPrintString("State: WON\r\n");
            break;
        case GAME_OVER:
            debugPrintString("State: GAME_OVER\r\n");
            break;
        default:
            debugPrintString("State: UNKNOWN\r\n");
            break;
    }
}

void debugPrintBoard(int board[BOARD_SIZE][BOARD_SIZE])
{
    debugPrintString("\r\n=== GAME BOARD ===\r\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        debugPrintString("| ");
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 0)
                debugPrintString("   0");
            else
            {
                char buffer[8];
                sprintf(buffer, "%4d", board[i][j]);
                debugPrintString(buffer);
            }
            debugPrintString(" | ");
        }
        debugPrintString("\r\n");
    }
    debugPrintString("==================\r\n");
}

void initGame(void)
{
    debugPrintString("\r\n*** INIT GAME 2048 ***\r\n");

    initRandom();

    // Clear board
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            gameBoard[i][j] = 0;

    currentGameState = GAME_PLAYING;
    debugPrintGameState(currentGameState);

    debugPrintString("Spawning initial tiles...\r\n");
    spawnRandomTile(gameBoard);
    spawnRandomTile(gameBoard);

    debugPrintBoard(gameBoard);

    debugPrintString("Game initialized successfully!\r\n");
}

void handleInputDirection(int direction)
{
    debugPrintString("\r\n--- HANDLE INPUT ---\r\n");
    debugPrintString("Direction: ");

    switch(direction)
    {
        case 0: debugPrintString("LEFT\r\n"); break;
        case 1: debugPrintString("RIGHT\r\n"); break;
        case 2: debugPrintString("UP\r\n"); break;
        case 3: debugPrintString("DOWN\r\n"); break;
        default: debugPrintString("INVALID\r\n"); return;
    }

    // Don't allow moves if game is over
    if (currentGameState == GAME_OVER) {
        debugPrintString("ERROR: Game Over! No more moves possible.\r\n");
        return;
    }

    if (currentGameState == GAME_WON) {
        debugPrintString("INFO: You won! Continue playing...\r\n");
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
        debugPrintString("Move successful! Spawning new tile...\r\n");
        spawnRandomTile(gameBoard);

        // Check win condition
        if (currentGameState == GAME_PLAYING && checkWin(gameBoard)) {
            currentGameState = GAME_WON;
            debugPrintString("*** CONGRATULATIONS! YOU REACHED 2048! ***\r\n");
        }

        // Check game over condition
        if (checkGameOver(gameBoard)) {
            currentGameState = GAME_OVER;
            debugPrintString("*** GAME OVER! NO MORE MOVES POSSIBLE ***\r\n");
        }

        debugPrintGameState(currentGameState);
        debugPrintBoard(gameBoard);
    }
    else
    {
        debugPrintString("Move failed! No tiles moved.\r\n");
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
        if (temp[i] != 0 && temp[i] == temp[i + 1])
        {
            temp[i] *= 2;
            score += temp[i];
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

        // Debug output
        debugPrintString("New tile spawned: ");
        debugPrintNumber(board[i][j]);
        debugPrintString(" at position (");
        debugPrintNumber(i);
        debugPrintString(",");
        debugPrintNumber(j);
        debugPrintString(")\r\n");
    }
    else
    {
        debugPrintString("WARNING: No empty cells to spawn tile!\r\n");
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
                debugPrintString("WIN CONDITION: Found tile ");
                debugPrintNumber(board[i][j]);
                debugPrintString(" at (");
                debugPrintNumber(i);
                debugPrintString(",");
                debugPrintNumber(j);
                debugPrintString(")\r\n");
                return true;
            }
        }
    }
    return false;
}

bool checkGameOver(int board[BOARD_SIZE][BOARD_SIZE])
{
    // If there are empty cells, game is not over
    if (hasEmptyCell(board)) {
        debugPrintString("Game continues: Empty cells available\r\n");
        return false;
    }

    // If no empty cells but can still merge, game is not over
    if (canMerge(board)) {
        debugPrintString("Game continues: Merge possible\r\n");
        return false;
    }

    // No empty cells and no possible merges = game over
    debugPrintString("GAME OVER: No empty cells and no merges possible\r\n");
    return true;
}

bool hasEmptyCell(int board[BOARD_SIZE][BOARD_SIZE])
{
    int emptyCount = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 0)
                emptyCount++;
        }
    }

    debugPrintString("Empty cells: ");
    debugPrintNumber(emptyCount);
    debugPrintString("\r\n");

    return emptyCount > 0;
}

bool canMerge(int board[BOARD_SIZE][BOARD_SIZE])
{
    int mergeCount = 0;

    // Check horizontal merges
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 1; j++)
        {
            if (board[i][j] == board[i][j + 1] && board[i][j] != 0)
                mergeCount++;
        }
    }

    // Check vertical merges
    for (int i = 0; i < BOARD_SIZE - 1; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == board[i + 1][j] && board[i][j] != 0)
                mergeCount++;
        }
    }

    debugPrintString("Possible merges: ");
    debugPrintNumber(mergeCount);
    debugPrintString("\r\n");

    return mergeCount > 0;
}

int (*getGameBoard(void))[BOARD_SIZE]
{
    return gameBoard;
}
