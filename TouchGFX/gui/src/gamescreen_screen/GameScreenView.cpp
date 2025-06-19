#include <gui/gamescreen_screen/GameScreenView.hpp>

#define BOARD_SIZE 4

GameScreenView* gameViewInstance = nullptr;

extern "C" {
    #include <stdbool.h>
    extern volatile bool shouldUpdate;
    extern int currentGameState;
    extern int score;

    extern int (*getGameBoard(void))[BOARD_SIZE];
    extern void initGame();

}

GameScreenView::GameScreenView()
{

}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();
    Square.initialize();
    gameViewInstance = this;
    initGame();
}

void GameScreenView::handleTickEvent()
{
	GameScreenViewBase::handleTickEvent();

	    if (shouldUpdate)
	    {
	    	switch(currentGameState)
			{
	    		case 1:
	    			WinGame();
	    			break;

	    		case 2:
	    			GameOver();
	    			break;
			}

	        shouldUpdate = false;
	        int (*board)[BOARD_SIZE] = getGameBoard();
	        for (int row = 0; row < BOARD_SIZE; row++)
	                for (int col = 0; col < BOARD_SIZE; col++)
	                    Square.updateTile(row, col, board[row][col]);
	    }
}


void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}

void GameScreenView::NewGame()
{
	gameOver.setVisible(false);
	gameOver.invalidate();
	winGame.setVisible(false);
	winGame.invalidate();
	initGame();
}

void GameScreenView::WinGame()
{
	winGame.setVisible(true);
	winGame.invalidate();
}

void GameScreenView::GameOver()
{
	gameOver.setVisible(true);
	gameOver.invalidate();
}

void GameScreenView::updateScore(int score)
{
	Unicode::snprintf(scoreBuffer, sizeof(scoreBuffer)/sizeof(scoreBuffer[0]), "%d", score);
	Score.setWildcard(scoreBuffer);
	Score.invalidate();
}


