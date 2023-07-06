#include "Level.h"

void Level::SetCell(Entity *t, int x, int y)
{
    delete mCells[x][y];
    mCells[x][y] = t->Clone();
    mCells[x][y]->cPhysics->x = x;
    mCells[x][y]->cPhysics->y = y;
}

bool Level::AddRandomRoom()
{
    int x = (rand() % MAP_WIDTH / 2) * 2 + 1;
    int y = (rand() % MAP_HEIGHT / 2) * 2 + 1;
    int width = (rand() % 6) * 2 + 7,
        height = (rand() % 4) * 2 + 7;

    if (x + width >= MAP_WIDTH || y + height >= MAP_HEIGHT) { return false; }

    for (int i = x; i < x+width; ++i)
    {
        for (int j = y; j < y + height; ++j)
        {
            if (!(mCells[i][j]->cPhysics->blocksMovement))
            {
                return false;
            }
        }
    }

    AddRoom({ x, y}, { x + width, y + height });

    return true;
}

void Level::AddRoom(point topLeft, point bottomRight)
{
    Entity *floor = new Entity("base_floor");
    for (int x = topLeft.first +1 ; x < bottomRight.first -1; ++x)
    {
        for (int y = topLeft.second +1 ; y < bottomRight.second -1; ++y)
        {
            SetCell(floor, x, y);
        }
    }

    delete floor;
}

void Level::StartMaze(int x, int y, Entity *tile)
{
}

void Level::Fill(Entity *t)
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            SetCell(t, x, y);
        }
    }
}

Level::Level()
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            mCells[x][y] = nullptr;
        }
    }

    Entity *wall = new Entity("base_wall");

    Fill(wall);

    delete wall;

    RoomsAndMazes();
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

void Level::RoomsAndMazes()
{
    /* https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/ */
    for (int i = 0; i < 1000; ++i)
    {
        AddRandomRoom();
    }

}