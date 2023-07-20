#include "Entity.h"

Entity::Entity()
{
    mName = "";
    mUUID = GenerateUUID();

    cActor = nullptr;
    cFOV = nullptr;
    cPhysics = nullptr;
    cRender = nullptr;
    cLog = nullptr;
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
    if (cFOV != nullptr) { n->cFOV = new FOVComponent((*cFOV)); }
    if (cPhysics != nullptr) { n->cPhysics = new PhysicsComponent((*cPhysics)); }
    if (cRender != nullptr) { n->cRender = new RenderComponent((*cRender)); }

    
    return n;
}

void Entity::Copy(Entity *other)
{
    mName = other->mName;
    if (other->cActor != nullptr) { delete cActor; cActor = new ActorComponent((*cActor)); }
    if (other->cFOV != nullptr) { delete cFOV; cFOV = new FOVComponent((*cFOV)); }
    if (other->cPhysics != nullptr) { delete cPhysics; cPhysics = new PhysicsComponent((*cPhysics)); }
    if (other->cRender != nullptr) { delete cRender; cRender = new RenderComponent((*cRender)); }
}

void Entity::Reset()
{
    mName = "";
    mUUID = GenerateUUID();
    delete cActor;
    cActor = nullptr;
    delete cFOV;
    cFOV = nullptr;
    delete cPhysics;
    cFOV = nullptr;
    delete cRender;
    cRender = nullptr;
    delete cLog;
    cLog = nullptr;
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
            else if (componentName == "FOV") { AddComponent<FOVComponent>((Component **)(&(cFOV)), data); }
            else if (componentName == "log") { AddComponent<LogComponent>((Component **)(&(cLog)), data); }
            else if (componentName == "actor") { AddComponent<ActorComponent>((Component **)(&(cActor)), data); }

        }
    }
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

bool Entity::IsAlive()
{
    return (GetHealth() > 0);
}

void Entity::TakeDamage(int damage)
{
    if (cActor == nullptr) { return; }
    cActor->health -= damage;
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

void Entity::SendLog(std::string message)
{
    if (cLog == nullptr) { return; }
    cLog->logs.emplace(cLog->logs.begin(), message);

    while (cLog->logs.size() > GUI_HEIGHT)
    {
        cLog->logs.pop_back();
    }
}
