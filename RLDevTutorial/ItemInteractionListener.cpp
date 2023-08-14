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
            Entity *target = nullptr;
            if (targetType == "nearestEnemy")
            {
                std::set<Entity *> entities = actorManager->GetEntities();

                double distance = 999999;

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

            }

            if (target == nullptr)
            {
                LogEvent l2(e->target, "Unable to find a target.");
                WorldFireEvent(&l2);
                return 0;
            }

            int amount = RollDamage(z.amount);

            if (z.type == "heal")
            {
            }
            else
            {
                DamageEvent d(e->target, target);
                d.damage = amount;
                d.type = z.type;
                WorldFireEvent(&d);
            }
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
