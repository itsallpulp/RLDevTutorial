#pragma once

#include "Entity.h"
#include "Event.h"

class Listener
{
	protected:
	long long mListenFor;
	bool ListensFor(int flag);
	void RegisterListenFor(int flag);
	int Fire(Event *e);

	virtual int FireMovementEvent(MovementEvent *e);

	public:
	Listener();
	int FireEvent(Event *e);
};