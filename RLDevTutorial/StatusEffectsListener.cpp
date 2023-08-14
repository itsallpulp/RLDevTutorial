#include "StatusEffectsListener.h"

int StatusEffectsListener::FireMovementEvent(MovementEvent *e)
{
    if (e->target->GetStatusEffect("confused") > 0)
    {
        std::cout << e->target->GetName() << " is confused! " << e->target->GetStatusEffect("confused") << std::endl;
        point dirs[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
        int i = rand() % 4;

        e->dx = dirs[i].first;
        e->dy = dirs[i].second;
    }

    return 0;
}

int StatusEffectsListener::FireAddStatusEffectEvent(AddStatusEffectEvent *e)
{
    std::cout << "Added status " << e->effect << " to " << e->target << " for " << e->rounds << " rounds!" << std::endl;
    e->target->AddStatusEffect(e->effect, e->rounds);
    return 0;
}

int StatusEffectsListener::FireTurnEvent(TurnEvent *e)
{
    e->target->ReduceEffects();
    return 0;
}

StatusEffectsListener::StatusEffectsListener()
{
    RegisterListenFor(evMove | evAddStatusEffect | evTakeTurn);
}