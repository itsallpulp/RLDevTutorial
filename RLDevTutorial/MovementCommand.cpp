#include "MovementCommand.h"
#include "Entity.h"
#include "EntityManager.h"

MovementCommand::MovementCommand(Entity *target, int dx, int dy)
{
    this->target = target;
    this->dx = dx;
    this->dy = dy;
}

int MovementCommand::Execute()
{
    MovementEvent e(target, dx, dy);

    /* If waiting, just wait */
    if (dx == 0 && dy == 0)
    {
        return WorldFireEvent(&e);
    }

    /* Is there someone where Entity is trying to go? */
    point p = target->GetXY();
    p.first += dx;
    p.second += dy;

    Entity *other;

    /* Yes, attack it */
    if ((other = actorManager->At(p.first, p.second)) != nullptr)
    {
        DamageEvent a(target, other);
        target->FigureDamage(&a);
        return WorldFireEvent(&a);
    }

    return WorldFireEvent(&e);
}
