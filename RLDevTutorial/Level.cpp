#include "Level.h"

Level::Level()
{
    Entity *floor = new Entity("base_floor");
    Entity *wall = new Entity("base_wall");

    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            mCells[x][y] = floor->Clone();
            mCells[x][y]->cPhysics->x = x;
            mCells[x][y]->cPhysics->y = y;
        }
    }

    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        delete mCells[x][0];
        mCells[x][0] = wall->Clone();
        mCells[x][0]->cPhysics->x = x;
        mCells[x][0]->cPhysics->y = 0;

        delete mCells[x][MAP_HEIGHT-1];
        mCells[x][MAP_HEIGHT-1] = wall->Clone();
        mCells[x][MAP_HEIGHT-1]->cPhysics->x = x;
        mCells[x][MAP_HEIGHT-1]->cPhysics->y = MAP_HEIGHT-1;
    }

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        delete mCells[0][y];
        mCells[0][y] = wall->Clone();
        mCells[0][y]->cPhysics->x = 0;
        mCells[0][y]->cPhysics->y = y;

        delete mCells[MAP_WIDTH-1][y];
        mCells[MAP_WIDTH-1][y] = wall->Clone();
        mCells[MAP_WIDTH-1][y]->cPhysics->x = MAP_WIDTH-1;
        mCells[MAP_WIDTH-1][y]->cPhysics->y = y;
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

Entity *Level::GetCell(int x, int y)
{
    return mCells[x][y];
}
