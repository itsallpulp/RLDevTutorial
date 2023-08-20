#include "ItemInteractionListener.h"
#include "EntityManager.h"

int ItemInteractionListener::FireGrabItemEvent(GrabItemEvent *e)
{
    LogEvent l(e->target, "You pick up the " + e->item->GetName() + ".");
    WorldFireEvent(&l);

    e->target->AddItem(e->item);
    itemManager->ChangeState(e->item, EntityManager::IN_INVENTORY);

    if (e->target == player)
    {
        itemManager->ToggleFlag(e->item, EntityManager::PERMANENT);
    }

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
            std::string type = z.type;

            if (z.type == "damage")
            {
                DamageEvent *d = new DamageEvent(e->target, nullptr);
                d->damage = RollDamage(z.amount);
                d->type = z.subtype;
                queuedEvents->push(d);
            }
            else if (z.type == "addStatusEffect")
            {
                AddStatusEffectEvent *s = new AddStatusEffectEvent(nullptr, z.subtype, RollDamage(z.amount));
                queuedEvents->push(s);
            }

            
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
                e->target->RemoveItem(e->consumed);
                itemManager->RemoveEntity(e->consumed);
                return 100;
            }
            else
            {
                LogEvent l(player, "There is no available target.");
                WorldFireEvent(&l);
                while (!queuedEvents->empty())
                {
                    Event *n = queuedEvents->front();
                    queuedEvents->pop();
                    delete n;
                }
                return 0;
            }
        }
        else if (targetType == "coordinate")
        {
            targeted = true;
            gameState = TARGETING;
            lookTarget = player->GetXY();
            return 0;
        }
    }
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

int ItemInteractionListener::FireRemoveItemEvent(RemoveItemEvent *e)
{
    e->holder->RemoveItem(e->item);
    itemManager->RemoveEntity(e->item);
    return 0;
}

int ItemInteractionListener::FireEquipItemEvent(EquipItemEvent *e)
{
    if (e->equipper->Equip(e->item)) 
    {
        LogEvent l(e->equipper, "You equip the " + e->item->GetName());
        WorldFireEvent(&l);
        return 100; 
    }
    LogEvent l(e->equipper, "You are unable to equip the " + e->item->GetName());
    WorldFireEvent(&l);
    return 0;
}

ItemInteractionListener::ItemInteractionListener()
{
    RegisterListenFor(evGrabItem | evConsumeItem | evDropItem | evRemoveItem | evEquipItem );
}
