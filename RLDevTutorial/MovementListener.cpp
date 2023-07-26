#include "MovementListener.h"

int MovementListener::FireMovementEvent(MovementEvent *e)
{
    if (e->target->cPhysics == nullptr) { return 0; }

    point dst = { e->target->cPhysics->x + e->dx, e->target->cPhysics->y + e->dy };

    if (level->GetCell(dst.first, dst.second)->cPhysics->blocksMovement) { return 0; } // Wall in the way

    e->target->cPhysics->x += e->dx;
    e->target->cPhysics->y += e->dy;
    /*
    std::string msg = "Move";
    int x = (dst.first * SPRITE_WIDTH) - ((msg.size() / 2) * SPRITE_WIDTH) + (SPRITE_WIDTH / 2);
    int y = dst.second * SPRITE_HEIGHT - SPRITE_HEIGHT;

    FloatingText text;
    text.color = 'y';
    text.msg = msg;
    text.opacity = 255;
    text.ticks = 0;
    text.x = x;
    text.y = y;

    floatingTexts->push_back(text);
    */

    return 0;
}

MovementListener::MovementListener() : Listener()
{
    RegisterListenFor(evMove);
}