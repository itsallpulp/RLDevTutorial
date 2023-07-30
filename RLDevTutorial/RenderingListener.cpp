#include "RenderingListener.h"
#include "Color.h"

int RenderingListener::FireRenderEvent(RenderEvent *e)
{
	//char bgColor = e->target->cRender->bgColor;
	int x = e->x == -1 ? e->target->cPhysics->x : e->x;
	int y = e->y == -1 ? e->target->cPhysics->y : e->y;

	char color = e->owColor == '-' ? e->target->cRender->color : e->owColor;
	int glyph = e->owGlyph == -1 ? e->target->cRender->glyph : e->owGlyph;
	char bgColor = e->owBg == '-' ? e->target->cRender->bgColor : e->owBg;

	if (bgColor == '~')
	{
		bgColor = level->GetCell(x, y)->cRender->bgColor;
	}

	x += GUI_WIDTH;

	if (e->darker)
	{
		int d = -45;
		Color c = colors[color];
		Color bgc = colors[bgColor];
		int r = std::max(c.r + d, 1);
		int g = std::max(c.g + d, 1);
		int b = std::max(c.b + d, 1);
		int br = std::max(bgc.r + d, 1);
		int bg = std::max(bgc.g + d, 1);
		int bb = std::max(bgc.b + d, 1);

		Render::Put(glyph, x, y, r, g, b, br, bg, bb);
	}
	else
	{
		Render::Put(glyph, x, y, color, bgColor);
	}

	return 0;
}

RenderingListener::RenderingListener()
{
	RegisterListenFor(evRender);
}
