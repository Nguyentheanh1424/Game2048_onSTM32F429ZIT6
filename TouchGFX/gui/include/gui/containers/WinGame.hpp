#ifndef WINGAME_HPP
#define WINGAME_HPP

#include <gui_generated/containers/WinGameBase.hpp>

class WinGame : public WinGameBase
{
public:
    WinGame();
    virtual ~WinGame() {}

    virtual void initialize();

    void RetryButtonClicked();
    void ContinueGame();

protected:
};

#endif // WINGAME_HPP
