#include "Entity.h"

#include "Level.h"

Entity::Entity()
{
    mName = "";
    mUUID = GenerateUUID();

    cActor = nullptr;
    cConsumable = nullptr;
    cEquippable = nullptr;
    cEquipment = nullptr;
    cFOV = nullptr;
    cInventory = nullptr;
    cPhysics = nullptr;
    cRender = nullptr;
    cLog = nullptr;
    cStatusEffects = nullptr;
}

Entity::Entity(std::string fileToLoad) : Entity()
{
    LoadJson(GetJson(fileToLoad));
}

Entity::~Entity()
{
    Reset();
}

Entity *Entity::Clone()
{
    Entity *n = new Entity();
    if (cActor != nullptr) { n->cActor = new ActorComponent((*cActor)); }
    if (cConsumable != nullptr) { n->cConsumable = new ConsumableComponent((*cConsumable)); }
    if (cEquippable != nullptr) { n->cEquippable = new EquippableComponent((*cEquippable)); }
    if (cEquipment != nullptr) { n->cEquipment = new EquipmentComponent((*cEquipment)); }
    if (cFOV != nullptr) { n->cFOV = new FOVComponent((*cFOV)); }
    if (cInventory != nullptr) { n->cInventory = new InventoryComponent((*cInventory)); }
    if (cPhysics != nullptr) { n->cPhysics = new PhysicsComponent((*cPhysics)); }
    if (cRender != nullptr) { n->cRender = new RenderComponent((*cRender)); }
    if (cStatusEffects != nullptr) { n->cStatusEffects = new StatusEffectsComponent((*cStatusEffects)); }

    n->mName = mName;
    
    return n;
}

void Entity::Copy(Entity *other)
{
    mName = other->mName;
    if (other->cActor != nullptr) { delete cActor; cActor = new ActorComponent(*(other->cActor)); }
    if (other->cConsumable != nullptr) { delete cConsumable; cConsumable = new ConsumableComponent(*(other->cConsumable)); }
    if (other->cEquippable != nullptr) { delete cEquippable; cEquippable = new EquippableComponent(*(other->cEquippable)); }
    if (other->cEquipment != nullptr) { delete cEquipment; cEquipment = new EquipmentComponent(*(other->cEquipment)); }
    if (other->cFOV != nullptr) { delete cFOV; cFOV = new FOVComponent(*(other->cFOV)); }
    if (other->cInventory != nullptr) { delete cInventory; cInventory = new InventoryComponent(*(other->cInventory)); }
    if (other->cPhysics != nullptr) { delete cPhysics; cPhysics = new PhysicsComponent(*(other->cPhysics)); }
    if (other->cRender != nullptr) { delete cRender; cRender = new RenderComponent(*(other->cRender)); }
    if (other->cStatusEffects != nullptr) { delete cStatusEffects; cStatusEffects = new StatusEffectsComponent(*(other->cStatusEffects)); }
}

void Entity::Reset()
{
    mName = "";
    mUUID = GenerateUUID();
    delete cActor;
    cActor = nullptr;
    delete cEquippable;
    cEquippable = nullptr;
    delete cEquipment;
    cEquipment = nullptr;
    delete cFOV;
    cFOV = nullptr;
    delete cConsumable;
    cConsumable = nullptr;
    delete cInventory;
    cInventory = nullptr;
    delete cPhysics;
    cPhysics = nullptr;
    cFOV = nullptr;
    delete cRender;
    cRender = nullptr;
    delete cLog;
    cLog = nullptr;
    delete cStatusEffects;
    cStatusEffects = nullptr;
}

std::string Entity::GetName()
{
    return mName;
}

std::string Entity::GetUUID()
{
    return mUUID;
}

void Entity::LoadJson(json::object data)
{
    if (data.contains("inherits"))
    {
        LoadJson(GetJson(json::value_to<std::string>(data["inherits"])));
    }


    mName = data.contains("name") ? json::value_to<std::string>(data["name"]) : mName;

    if (data.contains("components"))
    {
        json::object components = data["components"].as_object();

        for (json::object::value_type v : components)
        {
            std::string componentName = v.key();
            json::object data = v.value().as_object();

            if (componentName == "render") { AddComponent<RenderComponent>((Component **)(&(cRender)), data); }
            else if (componentName == "physics") { AddComponent<PhysicsComponent>((Component **)(&(cPhysics)), data); }
            else if (componentName == "equippable") { AddComponent<EquippableComponent>((Component **)(&(cEquippable)), data); }
            else if (componentName == "equipment") { AddComponent<EquipmentComponent>((Component **)(&(cEquipment)), data); }
            else if (componentName == "FOV") { AddComponent<FOVComponent>((Component **)(&(cFOV)), data); }
            else if (componentName == "log") { AddComponent<LogComponent>((Component **)(&(cLog)), data); }
            else if (componentName == "actor") { AddComponent<ActorComponent>((Component **)(&(cActor)), data); }
            else if (componentName == "inventory") { AddComponent<InventoryComponent>((Component **)(&(cInventory)), data); }
            else if (componentName == "consumable") { AddComponent<ConsumableComponent>((Component **)(&(cConsumable)), data); }

        }
    }
}

bool Entity::IsConsumable()
{
    return cConsumable != nullptr;
}

point Entity::GetHealthStats()
{
    point p = { 0,0 };

    if (cActor != nullptr)
    {
        p = { cActor->health, cActor->maxHealth };
    }

    return p;
}

int Entity::GetHealth()
{
    return cActor == nullptr ? 0 : cActor->health;
}

int Entity::GetMaxHealth()
{
    return cActor == nullptr ? 0 : cActor->maxHealth;
}

int Entity::Heal(int amount)
{
    if (cActor == nullptr) { return 0; }

    int healed = cActor->health;
    cActor->health = std::min(cActor->health + amount, cActor->maxHealth);

    healed = cActor->health - healed;

    return healed;
}

bool Entity::IsAlive()
{
    return (GetHealth() > 0);
}

void Entity::TakeDamage(int damage)
{
    if (cActor == nullptr) { return; }
    cActor->health -= damage;
}

int Entity::GetEnergy()
{
    return cActor == nullptr ? 0 : cActor->energy;
}

int Entity::ModEnergy(int d)
{
    return cActor == nullptr ? 0 : (cActor->energy += d);
}

bool Entity::HealsOnConsume()
{
    return cConsumable == nullptr ? false : (cConsumable->healAmount > 0);
}

int Entity::GetConsumeHealAmount()
{
    return cConsumable == nullptr ? 0 : cConsumable->healAmount;
}

std::string Entity::GetConsumeVerb()
{
    return cConsumable == nullptr ? " " : cConsumable->verb;
}

bool Entity::ZapsOnConsume()
{
    return cConsumable == nullptr ? false : (cConsumable->zaps.size() > 0);
}

std::vector<Zap> Entity::GetZaps()
{
    if (cConsumable != nullptr) { return cConsumable->zaps; }
    return std::vector<Zap>();
}

std::string Entity::GetTargetType()
{
    if (cConsumable != nullptr)
    {
        return cConsumable->targetType;
    }

    return "NA";
}

bool Entity::AddItem(Entity *item)
{
    if (cInventory == nullptr) { return false; }
    

    cInventory->AddItem(item);
    
    return true;
}

void Entity::RemoveItem(Entity *item)
{
    if (cInventory == nullptr) { return; }
    cInventory->RemoveItem(item);
}

std::vector<Entity *> Entity::GetInventory()
{
    if (cInventory != nullptr)
    {
        return cInventory->contents;
    }

    return std::vector<Entity *>();
}

point Entity::GetXY()
{
    point p = { -1,-1 };
    if (cPhysics != nullptr)
    {
        p = { cPhysics->x, cPhysics->y };
    }
    return p;
}

void Entity::SetXY(int x, int y)
{
    if (cPhysics != nullptr)
    {
        cPhysics->x = x;
        cPhysics->y = y;
    }
}

bool Entity::BlocksMovement()
{
    return cPhysics == nullptr ? false : cPhysics->blocksMovement;
}

bool Entity::BlocksVision()
{
    return cPhysics == nullptr ? false : cPhysics->blocksVision;
}

float Entity::GetWeight()
{
    return cPhysics == nullptr ? 0.0 : cPhysics->weight;
}

bool Entity::AllowDescend()
{
    return cPhysics == nullptr ? false : cPhysics->allowDescend;
}

void Entity::SendLog(std::string message)
{
    if (cLog == nullptr) { return; }
    cLog->logs.emplace(cLog->logs.begin(), message);

    while (cLog->logs.size() >= GUI_HEIGHT-1)
    {
        cLog->logs.pop_back();
    }
}

void Entity::FigureDamage(DamageEvent *e)
{
    if (cActor == nullptr)
    {
        return;
    }
    e->damage = RollDamage(cActor->baseDamage);
}

bool Entity::CanSee(Entity *other)
{
    point start = GetXY(), end = other->GetXY();

    int deltaX = end.first - start.first;
    signed char const ix = ((deltaX > 0) - (deltaX < 0));
    deltaX = std::abs(deltaX) << 1;

    int deltaY = end.second - start.second;
    signed char const iy = ((deltaY > 0) - (deltaY < 0));
    deltaY = std::abs(deltaY) << 1;

    int maxMonsterSight = 15;

    int X2 = end.first,
        X1 = start.first,
        Y2 = end.second,
        Y1 = start.second,
        count = 0;

    if (deltaX >= deltaY)
    {
        int error = deltaY - (deltaX >> 1);


        while (X1 != X2)
        {
            if ((error > 0) || (!error && (ix > 0)))
            {
                error -= deltaX;
                Y1 += iy;
            }

            error += deltaY;
            X1 += ix;
            ++count;

            if (level->GetCell(X1, Y1)->BlocksVision() || count > 25) { return false; }
        }
    }
    else
    {
        int error = deltaX - (deltaY >> 1);

        while (Y1 != Y2)
        {
            if ((error > 0) || (!error && (iy > 0)))
            {
                error -= deltaY;
                X1 += ix;
            }

            error += deltaX;
            Y1 += iy;
            ++count;

            if (level->GetCell(X1, Y1)->BlocksVision() || count > 25) { return false; }
        }
    }

    return true;
}

int Entity::GetStatusEffect(std::string effect)
{
    if (cStatusEffects == nullptr) { return 0; }

    if (cStatusEffects->effects.find(effect) == cStatusEffects->effects.end()) { return 0; }

    return cStatusEffects->effects[effect];
}

void Entity::AddStatusEffect(std::string effect, int rounds)
{
    if (cStatusEffects == nullptr)
    {
        cStatusEffects = new StatusEffectsComponent();
    }

    cStatusEffects->effects[effect] = rounds;
}

void Entity::ReduceEffects()
{
    if (cStatusEffects != nullptr)
    {
        std::cout << "Reducing status effects for " << mName << std::endl;
        for (auto it : cStatusEffects->effects)
        {
            std::cout << it.first << std::endl;
            cStatusEffects->effects[it.first] = (it.second - 1);
        }
    }
}

bool Entity::IsEquippable()
{
    return (cEquippable != nullptr);
}

int Entity::GetEquippableSlot()
{
    return cEquippable == nullptr ? -1 : cEquippable->slot;
}

std::string Entity::GetEquippableSlotStr()
{
    if (cEquippable == nullptr) { return "NA"; }
    switch (cEquippable->slot)
    {
        case ARMOR:
            return "body";
        case HEAD:
            return "head";
        case OFF_HAND:
            return "off-hand";
        case MAIN_HAND:
            return "main hand";
        case BOOTS:
            return "feet";
        case GLOVES:
            return "hands";
        case RING:
            return "ring";
        case TWO_HAND:
            return "two-handed";
    }
    return "NA";
}

int Entity::GetDamage()
{
    return cEquippable == nullptr ? 0 : cEquippable->damage;
}

int Entity::GetArmor()
{
    return cEquippable == nullptr ? 0 : cEquippable->armor;
}

bool Entity::Equip(Entity *e)
{
    if (cEquipment == nullptr || cInventory == nullptr) { return false; }
    int slot = e->GetEquippableSlot();

    if (slot == TWO_HAND)
    {
        if (cEquipment->equippedItems[MAIN_HAND] != nullptr)
        {
            cInventory->AddItem(cEquipment->equippedItems[MAIN_HAND]);
        }
        if (cEquipment->equippedItems[OFF_HAND] != nullptr && cEquipment->equippedItems[OFF_HAND] != cEquipment->equippedItems[MAIN_HAND])
        {
            cInventory->AddItem(cEquipment->equippedItems[OFF_HAND]);
        }
        cEquipment->equippedItems[MAIN_HAND] = e;
        cEquipment->equippedItems[OFF_HAND] = e;
        cInventory->RemoveItem(e);
        return true;
    }

    if (cEquipment->equippedItems[slot] != nullptr)
    {
        cInventory->AddItem(cEquipment->equippedItems[slot]);
        cEquipment->equippedItems[slot] = e;
        cInventory->RemoveItem(e);
    }

    return true;
}

bool Entity::Unequip(Entity *e)
{
    return false;
}

Entity *Entity::GetEquippedItem(int slot)
{
    if (cEquipment == nullptr) { return nullptr; }

    if (slot == TWO_HAND) { return cEquipment->equippedItems[MAIN_HAND]; }
    return cEquipment->equippedItems[slot];
}
