#include <gui/containers/SquareContainer.hpp>

extern void uart_printf(const char* fmt, ...);

SquareContainer::SquareContainer()
{
	uart_printf("SquareContainer constructor called\r\n");
}

void SquareContainer::initialize()
{
    SquareContainerBase::initialize();
    uart_printf("SquareContainer::initialize() called\r\n");

    tileText[0][0] = &Text_00;
    tileText[0][1] = &Text_01;
    tileText[0][2] = &Text_02;
    tileText[0][3] = &Text_03;

    tileText[1][0] = &Text_10;
    tileText[1][1] = &Text_11;
    tileText[1][2] = &Text_12;
    tileText[1][3] = &Text_13;

    tileText[2][0] = &Text_20;
    tileText[2][1] = &Text_21;
    tileText[2][2] = &Text_22;
    tileText[2][3] = &Text_23;

    tileText[3][0] = &Text_30;
    tileText[3][1] = &Text_31;
    tileText[3][2] = &Text_32;
    tileText[3][3] = &Text_33;

    uart_printf("SquareContainer initialized tileText mapping\r\n");
}

void SquareContainer::updateTile(int row, int col, int value)
{
    if (row < 0 || row >= 4 || col < 0 || col >= 4)
    {
        uart_printf("updateTile: Invalid row/col: %d/%d\r\n", row, col);
        return;
    }

    auto* text = tileText[row][col];

    if (text == nullptr)
    {
        uart_printf("updateTile: tileText[%d][%d] is NULL!\r\n", row, col);
        return;
    }

    if (value == 0)
    {
        text->setVisible(false);
        uart_printf("updateTile: Tile[%d][%d] cleared\r\n", row, col);
    }
    else
    {
        Unicode::snprintf(textBuffer[row][col], 6, "%d", value);
        text->setWildcard(textBuffer[row][col]);
        text->setVisible(true);

        char debugBuf[10];
        Unicode::toUTF8(&textBuffer[row][col][0], reinterpret_cast<uint8_t*>(debugBuf), sizeof(debugBuf));
        uart_printf("Tile[%d][%d] = %d (buffer = \"%s\")\r\n", row, col, value, debugBuf);


    }

    text->invalidate();
}


