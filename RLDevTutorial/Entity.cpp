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

std::string Entity::GetName()
{
    return mName;
}

std::string Entity::GetUUID()
{
    return mUUID;
}
