/*
 * Game2048.h
 *
 *  Created on: Jun 13, 2025
 *      Author: nthea
 */

#ifndef APPLICATION_USER_GAME2048_H_
#define APPLICATION_USER_GAME2048_H_

#include <stdbool.h>

#define BOARD_SIZE 4
#define WIN_TILE 2048

// Game states
typedef enum {
    GAME_PLAYING = 0,
    GAME_WON = 1,
    GAME_OVER = 2
} GameState;

extern int gameBoard[BOARD_SIZE][BOARD_SIZE];
extern GameState currentGameState;

void initGame(void);
void handleInputDirection(int direction);

bool moveLeft(int board[BOARD_SIZE][BOARD_SIZE]);
bool moveRight(int board[BOARD_SIZE][BOARD_SIZE]);
bool moveUp(int board[BOARD_SIZE][BOARD_SIZE]);
bool moveDown(int board[BOARD_SIZE][BOARD_SIZE]);
void spawnRandomTile(int board[BOARD_SIZE][BOARD_SIZE]);

bool checkWin(int board[BOARD_SIZE][BOARD_SIZE]);
bool checkGameOver(int board[BOARD_SIZE][BOARD_SIZE]);
bool hasEmptyCell(int board[BOARD_SIZE][BOARD_SIZE]);
bool canMerge(int board[BOARD_SIZE][BOARD_SIZE]);
GameState getGameState(void);

void debugPrintBoard(int board[BOARD_SIZE][BOARD_SIZE]);
void debugPrintString(const char* str);
void debugPrintNumber(int num);
void debugPrintGameState(GameState state);


#endif /* APPLICATION_USER_GAME2048_H_ */
