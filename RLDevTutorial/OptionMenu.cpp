#include "OptionMenu.h"
#include "ExitMenuCommand.h"
#include <iomanip>

OptionMenu::OptionMenu(std::string title, int x, int y) : Menu(title)
{
	this->x = x;
	this->y = y;
}

Command *OptionMenu::GetCommand()
{
	Command *c = nullptr;

	while (c == nullptr)
	{
		SDL_Event input;
		while (SDL_PollEvent(&input))
		{
			if (input.type == SDL_KEYDOWN)
			{
				switch (input.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						c = new ExitMenuCommand();
						break;
					default:
						break;
				}
			}
		}
	}
	return c;

	
}

void OptionMenu::AddOption(std::string name, Command *command, int key)
{
	MenuOption o = { name, command };

	if (key != -1)
	{
		options[key] = o;
	}
	else
	{
		for (char c : name)
		{
			if (options.find(((int)c)) == options.end())
			{
				std::cout << c << std::endl;
				options[(int)c] = o;
				break;
			}
		}
	}

	width = std::max(width, (int)(name.size() + 7));
}

void OptionMenu::Render()
{
	RenderBackground();

	int i = 1;
	
	for (auto it : options)
	{
		char c = (char)(it.first);

		std::string cmd = "[";
		cmd.push_back(c);
		cmd.push_back(']');

		Render::Puts(cmd, x + 1, y + i, foreground, '~');
		Render::Puts(it.second.description, x + 5, y + i++, foreground, '~');
	}

}
