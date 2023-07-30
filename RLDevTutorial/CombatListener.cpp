#include "CombatListener.h"
#include "FloatingText.h"

int CombatListener::FireDamageEvent(DamageEvent *e)
{
    LogEvent attackerLog(e->attacker, "You hit the " + e->defender->GetName() + " for " + std::to_string(e->damage) + ".");
    LogEvent defenderLog(e->defender, "The " + e->attacker->GetName() + " hits you for " + std::to_string(e->damage) + ".");

    e->defender->TakeDamage(e->damage);

    WorldFireEvent(&attackerLog);
    WorldFireEvent(&defenderLog);

    point p = e->defender->GetXY();

    recentlyHit[e->defender] = FT_SLOW;

    return 100;
}

int CombatListener::FireRenderEvent(RenderEvent *e)
{
    if (recentlyHit.find(e->target) != recentlyHit.end())
    {
        e->owColor = 'r';
        e->owGlyph = '*';
        --recentlyHit[e->target];

        if (recentlyHit[e->target] <= 0)
        {
            recentlyHit.erase(e->target);
        }
    }
    return 0;
}

CombatListener::CombatListener()
{
    RegisterListenFor(evDamage | evRender);
}
