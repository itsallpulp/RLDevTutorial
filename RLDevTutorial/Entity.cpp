#include "Entity.h"

Entity::Entity()
{
    mName = "";
    mUUID = GenerateUUID();

    cFOV = nullptr;
    cPhysics = nullptr;
    cRender = nullptr;
}

Entity::Entity(std::string fileToLoad) : Entity()
{
    LoadJson(GetJson(fileToLoad));
}

Entity::~Entity()
{
    Reset();
    /*
    delete cFOV;
    delete cPhysics;
    delete cRender;
    */
}

Entity *Entity::Clone()
{
    Entity *n = new Entity();
    if (cFOV != nullptr) { n->cFOV = new FOVComponent((*cFOV)); }
    if (cPhysics != nullptr) { n->cPhysics = new PhysicsComponent((*cPhysics)); }
    if (cRender != nullptr) { n->cRender = new RenderComponent((*cRender)); }

    
    return n;
}

void Entity::Copy(Entity *other)
{
    mName = other->mName;
    if (other->cFOV != nullptr) { delete cFOV; cFOV = new FOVComponent((*cFOV)); }
    if (other->cPhysics != nullptr) { delete cPhysics; cPhysics = new PhysicsComponent((*cPhysics)); }
    if (other->cRender != nullptr) { delete cRender; cRender = new RenderComponent((*cRender)); }
}

void Entity::Reset()
{
    mName = "";
    mUUID = GenerateUUID();
    delete cFOV;
    delete cPhysics;
    delete cRender;
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

        }
    }
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
