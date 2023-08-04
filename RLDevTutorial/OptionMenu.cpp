#include "OptionMenu.h"
#include "ExitMenuCommand.h"

OptionMenu::OptionMenu(std::string title, int x, int y) : Menu(title)
{
	this->x = x;
	this->y = y;
	AddOption("Exit", new ExitMenuCommand(), SDLK_ESCAPE);
}

OptionMenu::~OptionMenu()
{
	for (auto it : options)
	{
		delete it.second.command;
	}
}

Command *OptionMenu::GetCommand()
{
	while (true)
	{
		SDL_Event input;
		while (SDL_PollEvent(&input))
		{
			if (input.type == SDL_KEYDOWN)
			{
				if (options.find(input.key.keysym.sym) != options.end())
				{
					return options[input.key.keysym.sym].command;
				}
			}
		}
		RenderAll();
	}
	return nullptr;
}

void OptionMenu::AddOption(std::string name, Command *command, int key)
{
	MenuOption o = { name, command };

	if (key != -1 && options.count(key) == 0)
	{
		options[key] = o;
	}
	else
	{
		bool placed = false;
		for (char c : name)
		{
			if (options.find(((int)c)) == options.end())
			{
				options[(int)c] = o;
				placed = true;
				break;
			}
		}

		if (!placed)
		{
			for (int i = 65; i < 123; ++i)
			{
				if (options.find(i) == options.end())
				{
					options[i] = o;
					break;
				}
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
		if (it.first == SDLK_ESCAPE) { continue; }

		char c = (char)(it.first);

		std::string cmd = "[";
		cmd.push_back(c);
		cmd.push_back(']');

		Render::Puts(cmd, x + 1, y + i, foreground, '~');
		Render::Puts(it.second.description, x + 5, y + i++, foreground, '~');
	}

}
