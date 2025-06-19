#ifndef SQUARECONTAINER_HPP
#define SQUARECONTAINER_HPP

#include <gui_generated/containers/SquareContainerBase.hpp>

class SquareContainer : public SquareContainerBase
{
public:
	Unicode::UnicodeChar textBuffer[4][4][6];

	touchgfx::Box* tileBackground[4][4];
	touchgfx::FadeAnimator<touchgfx::TextAreaWithOneWildcard>* tileText[4][4];

    SquareContainer();
    virtual ~SquareContainer() {}

    virtual void initialize();

    void updateTile(int row, int col, int value);

protected:
};

#endif // SQUARECONTAINER_HPP
