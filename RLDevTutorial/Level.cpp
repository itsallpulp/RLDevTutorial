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
    int x = (rand() % MAP_WIDTH / 2) * 2;
    if (x % 2 != 0) { ++x; }
    int y = (rand() % MAP_HEIGHT / 2) * 2;
    if (y % 2 != 0) { ++y; }
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

void Level::CarveMaze(int x, int y, Entity *tile)
{
    SetCell(tile, x, y);
 
    std::vector<point> directions = {
        {1,0},
        {-1, 0},
        {0, 1},
        {0,-1}
    };

    while (!directions.empty())
    {
        auto it = directions.begin();
        std::advance(it, rand() % directions.size());
        if (MazeCanGo(x, y, (*it).first, (*it).second))
        {
            SetCell(tile, x + (*it).first, y + (*it).second);
            CarveMaze(x + (*it).first*2, y + (*it).second*2, tile);
        }
        directions.erase(it);
    }

    /*
    if (MazeCanGo(x, y, 1, 0)) 
    {
        SetCell(tile, x + 1, y);
        CarveMaze(x + 2, y, tile);
    }
    if (MazeCanGo(x, y, 0, 1))
    {
        SetCell(tile, x, y + 1);
        CarveMaze(x, y + 2, tile);
    }
    */
    /*
    std::queue<point> unchecked;
    unchecked.push({ x,y });

    while (!unchecked.empty())
    {
        std::cout << unchecked.size() << std::endl;
        point p = unchecked.front();
        unchecked.pop();

        if (!mCells[p.first][p.second]->cPhysics->blocksMovement) { continue; }

        SetCell(tile, p.first, p.second);

        //if (MazeCanGo(p.first, p.second, 1, 0)) { unchecked.push({ p.first + 1, p.second }); }
        //if (MazeCanGo(p.first, p.second, -1, 0)) { unchecked.push({ p.first - 1, p.second }); }
        if (MazeCanGo(p.first, p.second, 0, 1)) { unchecked.push({ p.first, p.second + 1}); }

    }
    */
}

int Level::CountSurroundingFloors(int x, int y)
{
    int n = 0;
    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            if (!mCells[x + i][y + j]->cPhysics->blocksMovement)
            {
                ++n;
            }
        }
    }
    return n;
}

bool Level::MazeCanGo(int x, int y, int dx, int dy)
{
    if (x + dx*2 >= MAP_WIDTH - 1) { return false; }
    if (x + dx*2  <= 0) { return false; }
    if (y + dy * 2 >= MAP_HEIGHT) { return false; }
    if (y + dy * 2 <= 0) { return false; }

    return (mCells[x + (dx * 2)][y + (dy * 2)]->cPhysics->blocksMovement);
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

    Entity *hall = new Entity("base_floor");

    for (int x = 1; x < MAP_WIDTH - 1; ++x)
    {
        for (int y = 1; y < MAP_HEIGHT - 1; ++y)
        {
            if (mCells[x][y]->cPhysics->blocksMovement && CountSurroundingFloors(x,y) == 0)
            {
                CarveMaze(x, y, hall);
            }
        }
    }

    delete hall;
}