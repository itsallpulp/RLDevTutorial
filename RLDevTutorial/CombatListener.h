#pragma once
#include "Listener.h"
class CombatListener : public Listener
{
	private:
	int FireDamageEvent(DamageEvent *e);
	
	public:
	CombatListener();
};