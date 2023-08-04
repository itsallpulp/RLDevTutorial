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
	virtual int FireLogEvent(LogEvent *e) { return 0; }
	virtual int FireDamageEvent(DamageEvent *e) { return 0; }
	virtual int FireTurnEvent(TurnEvent *e) { return 0; }
	virtual int FireGrabItemEvent(GrabItemEvent *e) { return 0; }
	virtual int FireDropItemEvent(DropItemEvent *e) { return 0; }
	virtual int FireConsumeItemEvent(ConsumeItemEvent *e) { return 0; }

	public:
	Listener();
	int FireEvent(Event *e);
};