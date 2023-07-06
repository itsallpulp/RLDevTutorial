#include "RenderingListener.h"

int RenderingListener::FireRenderEvent(RenderEvent *e)
{
	char bgColor = e->target->cRender->bgColor;

	if (bgColor == '~')
	{
		bgColor = level->GetCell(e->target->cPhysics->x, e->target->cPhysics->y)->cRender->bgColor;
	}

	Render::Put(e->target->cRender->glyph, e->target->cPhysics->x, e->target->cPhysics->y, e->target->cRender->color, bgColor);
	return 0;
}

RenderingListener::RenderingListener()
{
	RegisterListenFor(evRender);
}
