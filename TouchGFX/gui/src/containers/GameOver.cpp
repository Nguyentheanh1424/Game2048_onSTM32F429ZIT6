#include <gui/containers/GameOver.hpp>

#include <gui/gamescreen_screen/GameScreenView.hpp>
extern GameScreenView* gameViewInstance;

GameOver::GameOver()
{

}

void GameOver::initialize()
{
    GameOverBase::initialize();
}

void GameOver::RetryButtonClicked()
{
    if (gameViewInstance)
    {
        gameViewInstance->NewGame();
    }
}
