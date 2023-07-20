#pragma once
#include "Listener.h"
class CombatListener : public Listener
{
	private:
	int FireAttackEvent(AttackEvent *e);
	
	public:
	CombatListener();
};