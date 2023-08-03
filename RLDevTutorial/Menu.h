#pragma once

#include "Globals.h"
#include "Types.h"

class Menu
{
	private:
	int width, height, x, y;
	std::string title;
	char foreground, background;

	public:
	Menu(std::string title);
	void Render();
};