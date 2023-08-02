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

ItemInteractionListener::ItemInteractionListener()
{
    RegisterListenFor(evGrabItem);
}
