#include "Level.h"

Level::Level()
{
    Entity *floor = new Entity();
    floor->LoadJson(GetJson("base_floor"));

    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            mCells[x][y] = floor->Clone();
            mCells[x][y]->cPhysics->x = x;
            mCells[x][y]->cPhysics->y = y;
        }
    }
}

Level::Level(json::object toLoad)
{
}

json::object Level::ToJson()
{
    return json::object();
}

void Level::LoadJson(json::object data)
{
}

void Level::Render(int xOff, int yOff)
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            RenderEvent e(mCells[x][y]);
            FireEvent(&e);
        }
    }
}