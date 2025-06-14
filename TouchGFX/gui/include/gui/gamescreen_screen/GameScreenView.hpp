#ifndef GAMESCREENVIEW_HPP
#define GAMESCREENVIEW_HPP
#define BOARD_SIZE 4

#include <gui_generated/gamescreen_screen/GameScreenViewBase.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>

class GameScreenView : public GameScreenViewBase
{
public:
    GameScreenView();
    virtual ~GameScreenView() {}

    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateBoard(int board[BOARD_SIZE][BOARD_SIZE]);

protected:
};

#endif // GAMESCREENVIEW_HPP
