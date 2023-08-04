#pragma once

#include "Globals.h"
#include "Types.h"
#include "Command.h"

struct MenuOption {
	std::string description;
	Command *command;
};

class Menu
{
	protected:
	int width, height, x, y;
	std::string title;
	char foreground, background;
	void RenderBackground();

	public:
	Menu(std::string title);
	virtual void Render() { RenderBackground(); };
	virtual Command *GetCommand() { return nullptr; }
};