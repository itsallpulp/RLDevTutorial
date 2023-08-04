#pragma once
#include "Listener.h"
class CombatListener : public Listener
{
	private:
	int FireDamageEvent(DamageEvent *e);
	int FireRenderEvent(RenderEvent *e);
	int FireHealEvent(HealEvent *e);

	std::map<Entity *, int> recentlyHit;
	
	public:
	CombatListener();
};