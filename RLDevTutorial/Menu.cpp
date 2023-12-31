#include "Menu.h"

Menu::Menu(std::string title)
{
	this->title = title;
	x = 10;
	y = 10;
	width = title.size() + 4;
	height = 10;

	foreground = 'y';
	background = 'x';
}

void Menu::RenderBackground()
{
	Render::PutTitledBorder(title, x, y, width, height, foreground, background, FILL_BACKGROUND | BORDER_TITLE_CENTER);
}
