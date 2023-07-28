#include "DeathListener.h"
#include "EntityManager.h"
#include "FloatingText.h"

int DeathListener::FireDamageEvent(DamageEvent *e)
{
    if (!(e->defender->IsAlive()))
    {
        AddFloatingText(253, 'w', e->defender->GetXY());
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
