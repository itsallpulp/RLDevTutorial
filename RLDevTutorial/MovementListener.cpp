#include "MovementListener.h"

int MovementListener::FireMovementEvent(MovementEvent *e)
{
    if (e->target->cPhysics == nullptr) { return 0; }

    point dst = { e->target->cPhysics->x + e->dx, e->target->cPhysics->y + e->dy };

    if (level->GetCell(dst.first, dst.second)->cPhysics->blocksMovement) { return 0; } // Wall in the way

    e->target->cPhysics->x += e->dx;
    e->target->cPhysics->y += e->dy;

    if (e->target == player && level->GetCell(player->cPhysics->x, player->cPhysics->y)->AllowDescend())
    {
        Descend();
    }

    return 100;
}

MovementListener::MovementListener() : Listener()
{
    RegisterListenFor(evMove);
}