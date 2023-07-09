#include "Level.h"

std::vector<point> Level::GetPossibleDoors(int regionNumber)
{
    std::vector<point> doors;
    for (int x = 1; x < MAP_WIDTH - 1; ++x)
    {
        for (int y = 1; y < MAP_HEIGHT - 1; ++y)
        {
            if (mRegions[x][y] != regionNumber) { continue; }

            if (IsPossibleDoor(x, y, 1, 0)) { doors.push_back({ x + 1, y }); }
            if (IsPossibleDoor(x, y, -1, 0)) { doors.push_back({ x - 1, y }); }
            if (IsPossibleDoor(x, y, 0, 1)) { doors.push_back({ x, y + 1}); }
            if (IsPossibleDoor(x, y, 0, -1)) { doors.push_back({ x, y - 1}); }
        }
    }

    return doors;
}

bool Level::IsPossibleDoor(int x, int y, int dx, int dy)
{
    if (x + dx * 2 <= 0 || x + dx * 2 >= MAP_WIDTH - 1) { return false; }
    if (y + dy * 2 <= 0 || y + dy * 2 >= MAP_HEIGHT - 1) { return false; }

    return(
            (mCells[x + dx][y + dy]->cPhysics->blocksMovement &&  !mCells[x + dx * 2][y + dy * 2]->cPhysics->blocksMovement) && 
            (mRegions[x][y] != mRegions[x + dx * 2][y + dy * 2])
        );
}

void Level::RemoveDeadEnd(int x, int y, Entity *t)
{
    if (mCells[x][y]->cPhysics->blocksMovement) { return; }
    SetCell(t, x, y);
    if (!mCells[x+1][y]->cPhysics->blocksMovement && DiamondCountFloors(x+1, y) == 1) { RemoveDeadEnd(x+1, y, t); }
    if (!mCells[x-1][y]->cPhysics->blocksMovement && DiamondCountFloors(x-1, y) == 1) { RemoveDeadEnd(x-1, y, t); }
    if (!mCells[x][y+1]->cPhysics->blocksMovement && DiamondCountFloors(x, y+1) == 1) { RemoveDeadEnd(x, y+1, t); }
    if (!mCells[x][y-1]->cPhysics->blocksMovement && DiamondCountFloors(x, y-1) == 1) { RemoveDeadEnd(x, y-1, t); }

}

int Level::DiamondCountFloors(int x, int y)
{
    int n = 0;
    if (!mCells[x + 1][y]->cPhysics->blocksMovement) { ++n; }
    if (!mCells[x - 1][y]->cPhysics->blocksMovement) { ++n; }
    if (!mCells[x][y + 1]->cPhysics->blocksMovement) { ++n; }
    if (!mCells[x][y - 1]->cPhysics->blocksMovement) { ++n; }

    return n;
}

bool Level::RegionsAssimilated()
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            if (mRegions[x][y] > 1)
            {
                return false;
            }
        }
    }

    return true;
}

void Level::FloodfillRegion(int x, int y)
{
    if (mRegions[x][y] <= 1) { return; }
    mRegions[x][y] = 1;

    FloodfillRegion(x + 1, y);
    FloodfillRegion(x - 1, y);
    FloodfillRegion(x, y + 1);
    FloodfillRegion(x, y - 1);
}

void Level::SetCell(Entity *t, int x, int y)
{
    mCells[x][y] = t;
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
    ++nRegions;
    for (int x = topLeft.first +1 ; x < bottomRight.first -1; ++x)
    {
        for (int y = topLeft.second +1 ; y < bottomRight.second -1; ++y)
        {
            SetCell(mTiles[0], x, y);
            mRegions[x][y] = nRegions;
        }
    }

}

void Level::CarveMaze(int x, int y, Entity *tile)
{
    SetCell(tile, x, y);
    mRegions[x][y] = nRegions;
 
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
            mRegions[x + (*it).first][y + (*it).second] = nRegions;
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
            if (i == 0 && j == 0) { continue; }
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

void Level::ResetFOV()
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            fovMap[x][y] = fovHidden;
        }
    }
}

Level::Level()
{
    for (int i = 0; i < 255; ++i)
    {
        mTiles[i] = nullptr;
    }

    mTiles[0] = new Entity("base_floor");
    mTiles[1] = new Entity("base_wall");
    mTiles[2] = new Entity("base_door");

    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            mCells[x][y] = nullptr;
            mRegions[x][y] = 0;
        }
    }


    Fill(mTiles[1]);

    nRegions = 0;

    ResetFOV();
    //RoomsAndMazes();
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
            if (fovMap[x][y] == fovHidden) { continue; }

            RenderEvent e(mCells[x][y], x, y);
            FireEvent(&e);
        }
    }
}

Entity *Level::GetCell(int x, int y)
{
    return mCells[x][y];
}

void Level::ConnectRegion()
{
    if (RegionsAssimilated())
    {
        return;
    }


    std::vector<point> doors = GetPossibleDoors(1);

    auto it = doors.begin();
    std::advance(it, rand() % doors.size());
    SetCell(mTiles[2], (*it).first, (*it).second);


    mRegions[(*it).first][(*it).second] = 2;
    FloodfillRegion((*it).first, (*it).second);

    doors.erase(it);

    while (!doors.empty())
    {
        if (rand() % 55 == 1)
        {
            it = doors.begin();
            std::advance(it, rand() % doors.size());
            SetCell(mTiles[2], (*it).first, (*it).second);


            mRegions[(*it).first][(*it).second] = 2;
            FloodfillRegion((*it).first, (*it).second);
        }

        doors.pop_back();
    }

    doors.clear();

}

void Level::RoomsAndMazes(int roomPlacementAttempts)
{
    /* https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/ */

    ResetFOV();

    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            mRegions[x][y] = 0;
        }
    }

    Fill(mTiles[1]);
    nRegions = 0;

    for (int i = 0; i < roomPlacementAttempts; ++i)
    {
        AddRandomRoom();
    }


    for (int x = 1; x < MAP_WIDTH - 1; ++x)
    {
        for (int y = 1; y < MAP_HEIGHT - 1; ++y)
        {
            if (mCells[x][y]->cPhysics->blocksMovement && CountSurroundingFloors(x,y) == 0)
            {
                ++nRegions;
                CarveMaze(x, y, mTiles[0]);
            }
        }
    }
    
    while (!RegionsAssimilated())
    {
        ConnectRegion();
        /*
        std::vector<point> doors = GetPossibleDoors(1);

        auto it = doors.begin();
        std::advance(it, rand() % doors.size());
        SetCell(hall, (*it).first, (*it).second);

        std::cout << "Placed a door @ " << (*it).first << ", " << (*it).second << std::endl;

        mCells[(*it).first][(*it).second]->cRender->glyph = 43;
        mCells[(*it).first][(*it).second]->cRender->color = 'w';
        mCells[(*it).first][(*it).second]->cRender->bgColor = 'd';
        
        mRegions[(*it).first][(*it).second] = 2;
        FloodfillRegion((*it).first, (*it).second);

        doors.clear();*/
    }


    for (int x = 1; x < MAP_WIDTH - 1; ++x)
    {
        for (int y = 1; y < MAP_HEIGHT - 1; ++y)
        {
            if ((!mCells[x][y]->cPhysics->blocksMovement) && CountSurroundingFloors(x, y) == 1)
            {
                RemoveDeadEnd(x, y, mTiles[1]);
            }
        }
    }
    

    /*
    for (point p : doors)
    {
        mCells[p.first][p.second]->cRender->glyph = 249;
        mCells[p.first][p.second]->cRender->color = 'r';
    }*/
}

void Level::PlaceEntity(Entity *e)
{
    while (mCells[e->cPhysics->x][e->cPhysics->y]->cPhysics->blocksMovement)
    {
        e->cPhysics->x = rand() % MAP_WIDTH;
        e->cPhysics->y = rand() % MAP_HEIGHT;
    }
}

byte Level::GetFOV(int x, int y)
{
    return fovMap[x][y];
}

void Level::SetFOV(int value, int x, int y)
{
    fovMap[x][y] = (byte)(value);
}
