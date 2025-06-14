#include <gui/gamescreen_screen/GameScreenView.hpp>

#define BOARD_SIZE 4

GameScreenView* gameViewInstance = nullptr;

extern "C" void initGame();

GameScreenView::GameScreenView()
{

}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();
    gameViewInstance = this;
    initGame();
}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}

void GameScreenView::updateBoard(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
        	Square.updateTile(row, col, board[row][col]);
        }
    }
}

extern "C" void updateGameView(int board[BOARD_SIZE][BOARD_SIZE])
{
	if (gameViewInstance)
	{
		gameViewInstance->updateBoard(board);
	}
}
