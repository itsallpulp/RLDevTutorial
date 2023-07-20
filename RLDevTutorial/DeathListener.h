#pragma once
#include "Listener.h"
class DeathListener : public Listener
{
	private:
	int FireDamageEvent(DamageEvent *e);

	public:
	DeathListener();
};

