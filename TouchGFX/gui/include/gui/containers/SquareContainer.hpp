#ifndef SQUARECONTAINER_HPP
#define SQUARECONTAINER_HPP

#include <gui_generated/containers/SquareContainerBase.hpp>

class SquareContainer : public SquareContainerBase
{
public:
    SquareContainer();
    virtual ~SquareContainer() {}

    virtual void initialize();

    void updateTile(int row, int col, int value);

protected:
};

#endif // SQUARECONTAINER_HPP
