#include "ItemInteractionListener.h"
#include "EntityManager.h"

int ItemInteractionListener::FireGrabItemEvent(GrabItemEvent *e)
{
    LogEvent l(e->target, "You pick up the " + e->item->GetName() + ".");
    WorldFireEvent(&l);


    /* Need to make a clone of the item because it is a regular object in the entitymanager */
    Entity *item = e->item->Clone();
    e->target->AddItem(item);
    itemManager->RemoveEntity(e->item);

    return 100;
}

int ItemInteractionListener::FireConsumeItemEvent(ConsumeItemEvent *e)
{
    LogEvent l1(e->target, "You " + e->consumed->GetConsumeVerb() + " the " + e->consumed->GetName());
    WorldFireEvent(&l1);
    if (e->consumed->HealsOnConsume())
    {
        int healed = e->target->Heal(e->consumed->GetConsumeHealAmount());
        LogEvent log(e->target, "You regain " + std::to_string(healed) + " health.");
        WorldFireEvent(&log);
        AddFloatingText('+', 'g', e->target->GetXY());
    }

    e->target->RemoveItem(e->consumed);
    delete e->consumed;
    
    return 100;
}

ItemInteractionListener::ItemInteractionListener()
{
    RegisterListenFor(evGrabItem | evConsumeItem | 0);
}
