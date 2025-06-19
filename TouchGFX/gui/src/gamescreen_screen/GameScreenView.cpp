#include <gui/gamescreen_screen/GameScreenView.hpp>

#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"

#define BOARD_SIZE 4

GameScreenView* gameViewInstance = nullptr;

extern "C" void initGame();
extern UART_HandleTypeDef huart1;

void uart_printf(const char* fmt, ...)
{
    char buffer[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>(buffer), strlen(buffer), HAL_MAX_DELAY);
}

GameScreenView::GameScreenView()
{

}

void GameScreenView::setupScreen()
{
    uart_printf("setupScreen()\r\n");

    GameScreenViewBase::setupScreen();
    Square.initialize();
    uart_printf("Square initialized\r\n");

    gameViewInstance = this;
    uart_printf("gameViewInstance assigned\r\n");

    initGame();

    uart_printf("initGame() called\r\n");
}


void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}

void GameScreenView::updateBoard(int board[BOARD_SIZE][BOARD_SIZE])
{
    uart_printf("updateBoard() called\r\n");

    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            uart_printf("updateTile[%d][%d] = %d\r\n", row, col, board[row][col]);
            Square.updateTile(row, col, board[row][col]);
        }
    }
}


extern "C" void updateGameView(int board[BOARD_SIZE][BOARD_SIZE])
{
    uart_printf("updateGameView() called\r\n");
    if (gameViewInstance)
    {
        uart_printf("gameViewInstance is valid\r\n");
        gameViewInstance->updateBoard(board);
    }
    else
    {
        uart_printf("gameViewInstance is NULL!\r\n");
    }
}

