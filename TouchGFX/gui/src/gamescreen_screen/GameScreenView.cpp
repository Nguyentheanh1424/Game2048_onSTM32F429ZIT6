#include <gui/gamescreen_screen/GameScreenView.hpp>

#define BOARD_SIZE 4

GameScreenView* gameViewInstance = nullptr;

extern "C" {
    #include <stdbool.h>
	#include "main.h"
    extern volatile bool shouldUpdate;
    extern int currentGameState;
    extern int score;
    extern bool flagContinue;

    extern void SaveBestScore(int score);
    extern int LoadBestScore(void);
    extern int (*getGameBoard(void))[BOARD_SIZE];
    extern void initGame();
    extern void playAudio(int currentGameState);
}

GameScreenView::GameScreenView()
{

}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();

    Square.initialize();
    gameViewInstance = this;

    bestScore = LoadBestScore();
    Unicode::snprintf(BestScoreBuffer, 10, "%d", bestScore);
	BestScore.setWildcard(BestScoreBuffer);
	BestScore.invalidate();
    lastDisplayedScore = -1;

    flagContinue = false;

    initGame();

}



void GameScreenView::handleTickEvent()
{
	GameScreenViewBase::handleTickEvent();

	    if (shouldUpdate)
	    {
	    	shouldUpdate = false;

	    	updateScore(score);

	    	switch(currentGameState)
			{
	    		case 1:
	    			WinGame();
	    			break;

	    		case 2:
	    			GameOver();
	    			break;
			}
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

	flagContinue = false;

	playAudio(1);
	initGame();
}

void GameScreenView::WinGame()
{
	winGame.setVisible(true);
	playAudio(3);
	winGame.invalidate();
}

void GameScreenView::GameOver()
{
	gameOver.setVisible(true);
	playAudio(0);
	gameOver.invalidate();
}

void GameScreenView::Continue()
{
	winGame.setVisible(false);
	winGame.invalidate();
	flagContinue = true;
	currentGameState = 0;
}

void GameScreenView::updateScore(int currentScore)
{
    if (currentScore != lastDisplayedScore)
    {
        lastDisplayedScore = currentScore;

        Unicode::snprintf(ScoreBuffer, 10, "%d", currentScore);
        Score.setWildcard(ScoreBuffer);
        Score.invalidate();

        if (currentScore > bestScore)
        {
            bestScore = currentScore;

            Unicode::snprintf(BestScoreBuffer, 10, "%d", bestScore);
            BestScore.setWildcard(BestScoreBuffer);
            BestScore.invalidate();

            SaveBestScore(bestScore);
        }

    }
}




