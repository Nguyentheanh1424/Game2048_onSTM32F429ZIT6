#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include <gui_generated/containers/GameOverBase.hpp>

class GameOver : public GameOverBase
{
public:
    GameOver();
    virtual ~GameOver() {}

    virtual void initialize();

    void RetryButtonClicked();


protected:
};

#endif // GAMEOVER_HPP
