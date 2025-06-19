#include <gui/containers/SquareContainer.hpp>

#include <touchgfx/Color.hpp>

SquareContainer::SquareContainer()
{
}

void SquareContainer::initialize()
{
    SquareContainerBase::initialize();

    tileText[0][0] = &Text_00; tileBackground[0][0] = &Background_00;
	tileText[0][1] = &Text_01; tileBackground[0][1] = &Background_01;
	tileText[0][2] = &Text_02; tileBackground[0][2] = &Background_02;
	tileText[0][3] = &Text_03; tileBackground[0][3] = &Background_03;

	tileText[1][0] = &Text_10; tileBackground[1][0] = &Background_10;
	tileText[1][1] = &Text_11; tileBackground[1][1] = &Background_11;
	tileText[1][2] = &Text_12; tileBackground[1][2] = &Background_12;
	tileText[1][3] = &Text_13; tileBackground[1][3] = &Background_13;

	tileText[2][0] = &Text_20; tileBackground[2][0] = &Background_20;
	tileText[2][1] = &Text_21; tileBackground[2][1] = &Background_21;
	tileText[2][2] = &Text_22; tileBackground[2][2] = &Background_22;
	tileText[2][3] = &Text_23; tileBackground[2][3] = &Background_23;

	tileText[3][0] = &Text_30; tileBackground[3][0] = &Background_30;
	tileText[3][1] = &Text_31; tileBackground[3][1] = &Background_31;
	tileText[3][2] = &Text_32; tileBackground[3][2] = &Background_32;
	tileText[3][3] = &Text_33; tileBackground[3][3] = &Background_33;

}

void SquareContainer::updateTile(int row, int col, int value)
{
    auto* text = tileText[row][col];
    auto* bg = tileBackground[row][col];

    if (text == nullptr)
    {
        return;
    }

    if (value == 0)
    {
        text->setVisible(false);
        bg->setColor(touchgfx::Color::getColorFromRGB(205,193,180));
        bg->invalidate();
    }
    else
    {
        Unicode::snprintf(textBuffer[row][col], 6, "%d", value);
        text->setWildcard(textBuffer[row][col]);
        text->setVisible(true);
		if (bg)
		{
			colortype color = touchgfx::Color::getColorFromRGB(200, 200, 200);

			switch (value)
			{
				case 2:    color = touchgfx::Color::getColorFromRGB(150, 0, 255); break;
				case 4:    color = touchgfx::Color::getColorFromRGB(240, 20, 90); break;
				case 8:    color = touchgfx::Color::getColorFromRGB(255, 201, 27); break;
				case 16:   color = touchgfx::Color::getColorFromRGB(245, 149, 99); break;
				case 32:   color = touchgfx::Color::getColorFromRGB(0, 149, 214); break;
				case 64:   color = touchgfx::Color::getColorFromRGB(206, 0, 123); break;
				case 128:  color = touchgfx::Color::getColorFromRGB(255, 85, 24); break;
				case 256:  color = touchgfx::Color::getColorFromRGB(41, 215, 165); break;
				case 512:  color = touchgfx::Color::getColorFromRGB(255, 0, 36); break;
				case 1024: color = touchgfx::Color::getColorFromRGB(95, 6, 155); break;
			}
			bg->setColor(color);
			bg->invalidate();
		}
    }

    text->invalidate();
}




