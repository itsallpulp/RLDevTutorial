#include "MovementListener.h"

int MovementListener::FireMovementEvent(MovementEvent *e)
{
    if (e->target->cPhysics == nullptr) { return 0; }

    e->target->cPhysics->x += e->dx;
    e->target->cPhysics->y += e->dy;


    return 0;
}

MovementListener::MovementListener() : Listener()
{
    RegisterListenFor(evMove);
}