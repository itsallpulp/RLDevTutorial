#pragma once

#include "Entity.h"
#include "Event.h"
#include "Globals.h"

class Listener
{
	protected:
	long long mListenFor;
	bool ListensFor(int flag);
	void RegisterListenFor(int flag);

	virtual int FireMovementEvent(MovementEvent *e);
	virtual int FireRenderEvent(RenderEvent *e);

	public:
	Listener();
	int FireEvent(Event *e);
};