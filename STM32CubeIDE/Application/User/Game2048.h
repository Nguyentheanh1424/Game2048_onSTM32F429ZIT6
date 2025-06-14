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

extern int gameBoard[BOARD_SIZE][BOARD_SIZE];

void initGame(void);
void handleInputDirection(int direction);

bool moveLeft(int board[BOARD_SIZE][BOARD_SIZE]);
bool moveRight(int board[BOARD_SIZE][BOARD_SIZE]);
bool moveUp(int board[BOARD_SIZE][BOARD_SIZE]);
bool moveDown(int board[BOARD_SIZE][BOARD_SIZE]);
void spawnRandomTile(int board[BOARD_SIZE][BOARD_SIZE]);


#endif /* APPLICATION_USER_GAME2048_H_ */
