#pragma once
#include "Listener.h"
class StatusEffectsListener : public Listener
{
	private:
	int FireMovementEvent(MovementEvent *e);
	int FireAddStatusEffectEvent(AddStatusEffectEvent *e);
	int FireTurnEvent(TurnEvent *e);

	public:
	StatusEffectsListener();
};