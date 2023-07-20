#include "CombatListener.h"

int CombatListener::FireAttackEvent(AttackEvent *e)
{
    LogEvent attackerLog(e->attacker, "You hit the " + e->defender->GetName() + " for 1.");
    LogEvent defenderLog(e->defender, "The " + e->attacker->GetName() + " hits you for 1.");

    e->defender->TakeDamage(1);

    WorldFireEvent(&attackerLog);
    WorldFireEvent(&defenderLog);

    return 100;
}

CombatListener::CombatListener()
{
    RegisterListenFor(evAttack);
}
