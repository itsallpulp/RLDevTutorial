#include "Entity.h"

Entity::Entity()
{
    mName = "";
    mUUID = GenerateUUID();

    cPhysics = nullptr;
    cRender = nullptr;
}

Entity::~Entity()
{
    delete cPhysics;
    delete cRender;
}

Entity *Entity::Clone()
{
    Entity *n = new Entity();
    if (cPhysics != nullptr) (n->cPhysics = new PhysicsComponent((*cPhysics)));
    if (cRender != nullptr) (n->cRender = new RenderComponent((*cRender)));

    
    return n;
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

        }
    }
}
