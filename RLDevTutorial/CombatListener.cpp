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

    //AddFloatingText("*", 'r', p.first, p.second, FT_FAST);
    recentlyHit[e->defender] = FT_SLOW;
    //AddFloatingText(std::to_string(e->damage), 'r', p.first, p.second, FT_FAST);

    return 100;
}

int CombatListener::FireRenderEvent(RenderEvent *e)
{
    if (recentlyHit.find(e->target) != recentlyHit.end())
    {
        e->owColor = 'r';
        e->owGlyph = '/';
        //e->owBg = 'e';
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
