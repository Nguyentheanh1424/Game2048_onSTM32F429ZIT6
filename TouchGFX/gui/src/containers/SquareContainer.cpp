#include <gui/containers/SquareContainer.hpp>

SquareContainer::SquareContainer()
{

}

void SquareContainer::initialize()
{
    SquareContainerBase::initialize();
}

static Unicode::UnicodeChar textBuffer[4][4][6];

void SquareContainer::updateTile(int row, int col, int value)
{
    touchgfx::FadeAnimator<touchgfx::TextAreaWithOneWildcard>* tileText = nullptr;

    if      (row == 0 && col == 0) tileText = &Text_00;
    else if (row == 0 && col == 1) tileText = &Text_01;
    else if (row == 0 && col == 2) tileText = &Text_02;
    else if (row == 0 && col == 3) tileText = &Text_03;
    else if (row == 1 && col == 0) tileText = &Text_10;
    else if (row == 1 && col == 1) tileText = &Text_11;
    else if (row == 1 && col == 2) tileText = &Text_12;
    else if (row == 1 && col == 3) tileText = &Text_13;
    else if (row == 2 && col == 0) tileText = &Text_20;
    else if (row == 2 && col == 1) tileText = &Text_21;
    else if (row == 2 && col == 2) tileText = &Text_22;
    else if (row == 2 && col == 3) tileText = &Text_23;
    else if (row == 3 && col == 0) tileText = &Text_30;
    else if (row == 3 && col == 1) tileText = &Text_31;
    else if (row == 3 && col == 2) tileText = &Text_32;
    else if (row == 3 && col == 3) tileText = &Text_33;

    if (tileText)
    {
        if (value == 0)
        {
            tileText->setVisible(false);
        }
        else
        {
            Unicode::snprintf(textBuffer[row][col], 6, "%d", value);
            tileText->setWildcard(textBuffer[row][col]);
            tileText->setVisible(true);
        }
        tileText->invalidate();
    }
}
