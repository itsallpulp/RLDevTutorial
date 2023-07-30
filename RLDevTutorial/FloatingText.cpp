#include "FloatingText.h"
#include "Globals.h"
#include "Level.h"
#include "FOVData.h"

void AddFloatingText(std::string text, char color, int x, int y, int speed)
{
	if (level->GetFOV(x, y) != fovVisible) { return; }

	FloatingText t;
	t.msg = text;
	t.color = color;
	t.x = ((x+GUI_WIDTH) * SPRITE_WIDTH) - ((text.size() / 2) * SPRITE_WIDTH) + (SPRITE_WIDTH / 2);
	t.y = ((y - 1) * SPRITE_HEIGHT);
	t.opacity = 255;
	t.ticks = 0;
	t.speed = speed;
	floatingTexts->push_back(t);
}

void AddFloatingText(std::string text, char color, point p, int speed)
{
	AddFloatingText(text, color, p.first, p.second, speed);
}

void AddFloatingText(int glyph, char color, point p, int speed)
{
	int x = p.first, y = p.second;
	if (level->GetFOV(p.first, p.second) != fovVisible) { return; }

	FloatingText t;
	t.msg = "";
	t.glyph = glyph;
	t.color = color;
	t.x = ((x + GUI_WIDTH) * SPRITE_WIDTH) + (SPRITE_WIDTH / 2);
	t.y = ((y - 1) * SPRITE_HEIGHT);
	t.opacity = 255;
	t.ticks = 0;
	t.speed = speed;
	floatingTexts->push_back(t);
}