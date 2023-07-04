#include "MovementCommand.h"

MovementCommand::MovementCommand(Entity *target, int dx, int dy)
{
    this->target = target;
    this->dx = dx;
    this->dy = dy;
}

int MovementCommand::Execute()
{
    MovementEvent e(target, dx, dy);
    return FireEvent(&e);
}
