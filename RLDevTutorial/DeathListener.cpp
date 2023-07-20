#include "DeathListener.h"
#include "EntityManager.h"

int DeathListener::FireDamageEvent(DamageEvent *e)
{
    if (!(e->defender->IsAlive()))
    {
        LogEvent l(e->attacker, "You have slain the " + e->defender->GetName());
        actorManager->RemoveEntity(e->defender);
        WorldFireEvent(&l);
    }
    return 0;
}

DeathListener::DeathListener()
{
    RegisterListenFor(evDamage);
}
