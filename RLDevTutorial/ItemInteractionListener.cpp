#include "ItemInteractionListener.h"
#include "EntityManager.h"

int ItemInteractionListener::FireGrabItemEvent(GrabItemEvent *e)
{
    LogEvent l(e->target, "You pick up the " + e->item->GetName() + ".");
    WorldFireEvent(&l);

    e->target->AddItem(e->item);
    itemManager->ChangeState(e->item, EntityManager::IN_INVENTORY);
    //itemManager->RemoveEntity(e->item);

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
    itemManager->RemoveEntity(e->consumed);
    
    return 100;
}

int ItemInteractionListener::FireDropItemEvent(DropItemEvent *e)
{
    LogEvent l(e->target, "You drop the " + e->item->GetName() + ".");
    WorldFireEvent(&l);
    
    e->target->RemoveItem(e->item);
    point p = e->target->GetXY();
    e->item->SetXY(p.first, p.second);
    itemManager->ChangeState(e->item, EntityManager::ON_MAP);

    std::cout << p.first << ", " << p.second;
    p = e->item->GetXY();

    return 100;
}

ItemInteractionListener::ItemInteractionListener()
{
    RegisterListenFor(evGrabItem | evConsumeItem | evDropItem);
}
