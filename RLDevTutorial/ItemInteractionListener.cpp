#include "ItemInteractionListener.h"
#include "EntityManager.h"

int ItemInteractionListener::FireGrabItemEvent(GrabItemEvent *e)
{
    LogEvent l(e->target, "You pick up the " + e->item->GetName() + ".");
    WorldFireEvent(&l);

    e->target->AddItem(e->item);
    itemManager->ChangeState(e->item, EntityManager::IN_INVENTORY);

    return 100;
}

int ItemInteractionListener::FireConsumeItemEvent(ConsumeItemEvent *e)
{
    bool targeted = false;

    LogEvent l1(e->target, "You " + e->consumed->GetConsumeVerb() + " the " + e->consumed->GetName());
    WorldFireEvent(&l1);
    if (e->consumed->HealsOnConsume())
    {
        HealEvent h(e->target, RollDamage(e->consumed->GetConsumeHealAmount()));
        WorldFireEvent(&h);
    }

    if (e->consumed->ZapsOnConsume())
    {
        std::vector<Zap> zaps = e->consumed->GetZaps();
        std::string targetType = e->consumed->GetTargetType();

        for (Zap z : zaps)
        {
            DamageEvent *d = new DamageEvent(e->target, nullptr);
            d->damage = RollDamage(z.amount);
            d->type = z.type;
            queuedEvents->push(d);
        }

        queuedEvents->push(new RemoveItemEvent(e->target, e->consumed));
    
        if (targetType == "nearestEnemy")
        {
            std::set<Entity *> entities = actorManager->GetEntities();

            double distance = 999999;
            Entity *target = nullptr;

            for (Entity *entity : entities)
            {
                if (e->target != entity && e->target->CanSee(entity))
                {
                    double d = Distance(e->target->GetXY(), entity->GetXY());

                    if (d < distance)
                    {
                        distance = d;
                        target = entity;
                    }
                }
            }

            if (target != nullptr)
            {
                point p = target->GetXY();
                UnloadQueuedEvents(p.first, p.second);
            }
        }
        else if (targetType == "coordinate")
        {
            targeted = true;
            gameState = TARGETING;
            lookTarget = player->GetXY();
            std::cout << "gamestate switched to Targeting" << std::endl;
            return 0;
        }

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
