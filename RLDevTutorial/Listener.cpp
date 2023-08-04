#include "Listener.h"

bool Listener::ListensFor(int flag)
{
    return mListenFor & flag;
}

void Listener::RegisterListenFor(int flag)
{
    mListenFor |= flag;
}

int Listener::FireMovementEvent(MovementEvent *e)
{
    return 0;
}

int Listener::FireRenderEvent(RenderEvent *e)
{
    return 0;
}

Listener::Listener()
{
    mListenFor = 0;
}

int Listener::FireEvent(Event *e)
{
    if (!ListensFor(e->GetType())) { return 0; }
    switch (e->GetType())
    {
        case evMove:
            return FireMovementEvent((MovementEvent *)e);
        case evRender:
            return FireRenderEvent((RenderEvent *)e);
        case evLog:
            return FireLogEvent((LogEvent *)e);
        case evDamage:
            return FireDamageEvent((DamageEvent *)e);
        case evTakeTurn:
            return FireTurnEvent((TurnEvent *)e);
        case evGrabItem:
            return FireGrabItemEvent((GrabItemEvent *)e);
        case evConsumeItem:
            std::cout << "ConsumeItemEvent" << std::endl;
            return FireConsumeItemEvent((ConsumeItemEvent *)e);
        default:
            return 0;
    }

    return 0;
}
