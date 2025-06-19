#include <gui/containers/WinGame.hpp>

#include <gui/gamescreen_screen/GameScreenView.hpp>
extern GameScreenView* gameViewInstance;

WinGame::WinGame()
{

}

void WinGame::initialize()
{
    WinGameBase::initialize();
}

void WinGame::RetryButtonClicked()
{
    if (gameViewInstance)
    {
        gameViewInstance->NewGame();
    }
}

void WinGame::ContinueGame()
{
	if (gameViewInstance)
	{
		gameViewInstance->Continue();
	}
}
