#pragma once
#include "Menu.h"
class OptionMenu : public Menu
{
	protected:
	std::map<int, MenuOption> options;

	public:
	OptionMenu(std::string title, int x, int y);
	~OptionMenu();
	Command *GetCommand();
	void AddOption(std::string name, Command *command, int key=-1);
	void Render();
};

