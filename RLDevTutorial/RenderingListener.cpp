#include "RenderingListener.h"

int RenderingListener::FireRenderEvent(RenderEvent *e)
{
	Render::Put(e->target->cRender->glyph, e->target->cPhysics->x, e->target->cPhysics->y, e->target->cRender->color, e->target->cRender->bgColor);
	return 0;
}

RenderingListener::RenderingListener()
{
	RegisterListenFor(evRender);
}
