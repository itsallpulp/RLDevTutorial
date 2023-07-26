#include "CombatListener.h"

int CombatListener::FireDamageEvent(DamageEvent *e)
{
    LogEvent attackerLog(e->attacker, "You hit the " + e->defender->GetName() + " for " + std::to_string(e->damage) + ".");
    LogEvent defenderLog(e->defender, "The " + e->attacker->GetName() + " hits you for " + std::to_string(e->damage) + ".");

    e->defender->TakeDamage(e->damage);

    WorldFireEvent(&attackerLog);
    WorldFireEvent(&defenderLog);

    point p = e->defender->GetXY();

    AddFloatingText(std::to_string(e->damage), 'r', p.first, p.second, FT_FAST);

    return 100;
}

CombatListener::CombatListener()
{
    RegisterListenFor(evDamage);
}
