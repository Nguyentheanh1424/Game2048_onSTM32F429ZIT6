#ifndef GAMESCREENVIEW_HPP
#define GAMESCREENVIEW_HPP
#define BOARD_SIZE 4

#include <gui_generated/gamescreen_screen/GameScreenViewBase.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>

class GameScreenView : public GameScreenViewBase
{
public:
	int bestScore;
	int lastDisplayedScore;

    GameScreenView();
    virtual ~GameScreenView() {}

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    void NewGame();
    void WinGame();
    void GameOver();
    void Continue();

    void updateScore(int score);

protected:
};

#endif // GAMESCREENVIEW_HPP
