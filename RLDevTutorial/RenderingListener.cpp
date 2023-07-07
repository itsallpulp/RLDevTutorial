#include "RenderingListener.h"

int RenderingListener::FireRenderEvent(RenderEvent *e)
{
	char bgColor = e->target->cRender->bgColor;
	int x = e->x == -1 ? e->target->cPhysics->x : e->x;
	int y = e->y == -1 ? e->target->cPhysics->y : e->y;

	if (bgColor == '~')
	{
		bgColor = level->GetCell(x, y)->cRender->bgColor;
	}

	Render::Put(e->target->cRender->glyph, x, y, e->target->cRender->color, bgColor);
	return 0;
}

RenderingListener::RenderingListener()
{
	RegisterListenFor(evRender);
}
