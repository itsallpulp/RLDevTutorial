#include "RenderingListener.h"
#include "Color.h"

int RenderingListener::FireRenderEvent(RenderEvent *e)
{
	char bgColor = e->target->cRender->bgColor;
	int x = e->x == -1 ? e->target->cPhysics->x : e->x;
	int y = e->y == -1 ? e->target->cPhysics->y : e->y;

	if (bgColor == '~')
	{
		bgColor = level->GetCell(x, y)->cRender->bgColor;
	}

	if (e->darker)
	{
		int d = -45;
		Color c = colors[e->target->cRender->color];
		Color bgc = colors[bgColor];
		int r = std::max(c.r + d, 1);
		int g = std::max(c.g + d, 1);
		int b = std::max(c.b + d, 1);
		int br = std::max(bgc.r + d, 1);
		int bg = std::max(bgc.g + d, 1);
		int bb = std::max(bgc.b + d, 1);

		Render::Put(e->target->cRender->glyph, x, y, r, g, b, br, bg, bb);
	}
	else
	{
		Render::Put(e->target->cRender->glyph, x, y, e->target->cRender->color, bgColor);
	}

	return 0;
}

RenderingListener::RenderingListener()
{
	RegisterListenFor(evRender);
}
