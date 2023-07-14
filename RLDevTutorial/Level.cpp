#include "Level.h"
#include "Pathfinder.h"

std::vector<point> Level::GetPossibleDoors(int regionNumber)
{
    //auto start = std::chrono::high_resolution_clock::now();
    //std::cout << "Searching for possible doors...\t";

    std::vector<point> doors;
    for (int x = 1; x < MAP_WIDTH - 1; ++x)
    {
        for (int y = 1; y < MAP_HEIGHT - 1; ++y)
        {
            if (mRegions[x][y] != regionNumber) { continue; }

           // auto s1 = std::chrono::high_resolution_clock::now();
            if (IsPossibleDoor(x, y, 1, 0)) { doors.push_back({ x + 1, y }); }
            if (IsPossibleDoor(x, y, -1, 0)) { doors.push_back({ x - 1, y }); }
            if (IsPossibleDoor(x, y, 0, 1)) { doors.push_back({ x, y + 1}); }
            if (IsPossibleDoor(x, y, 0, -1)) { doors.push_back({ x, y - 1}); }
           /* auto s2 = std::chrono::high_resolution_clock::now();

            std::cout << "Checked " << x << ", " << y << " to be a door in " << std::chrono::duration_cast<std::chrono::microseconds>(s2 - s1).count() << "ms" << std::endl;*/
        }
    }
   /* auto stop = std::chrono::high_resolution_clock::now();
    long long total = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    std::cout << "Found " << doors.size() << " doors in " << total << "ms" << std::endl;*/

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

void Level::ConnectPoints()
{
    for (int x = 2; x < MAP_WIDTH - 2; ++x)//x+=(1 + rand()%5))
    {
        for (int y = 2; y < MAP_HEIGHT - 2; ++y)//y+=(1+rand()%5))
        {
            if (mCells[x][y]->BlocksMovement()) { continue; }

            if (mCells[x+1][y]->BlocksMovement() && !mCells[x+2][y]->BlocksMovement() && pathfinder->GeneratePath({ x,y }, { x+2,y }).size() > 25)
            {
                mCells[x+1][y] = mTiles[0];
            }
            else if (mCells[x-1][y]->BlocksMovement() && !mCells[x-2][y]->BlocksMovement() && pathfinder->GeneratePath({ x,y }, { x-2,y }).size() > 25)
            {
                mCells[x-1][y] = mTiles[0];
            }
            else if (mCells[x][y+1]->BlocksMovement() && !mCells[x][y+2]->BlocksMovement() && pathfinder->GeneratePath({ x,y }, { x,y+2 }).size() > 25)
            {
                mCells[x][y+1] = mTiles[0];
            }
            else if (mCells[x][y - 1]->BlocksMovement() && !mCells[x][y - 2]->BlocksMovement() && pathfinder->GeneratePath({ x,y }, { x,y - 2 }).size() > 25)
            {
                mCells[x][y - 1] = mTiles[0];
            }
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

Level::~Level()
{
    ClearCells();
}

void Level::ClearCells()
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            if (mCells[x][y] != nullptr)
            {
                delete mCells[x][y];
            }
            mCells[x][y] = nullptr;
        }
    }
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

            bool darker = (fovMap[x][y] == fovMemory);

            //if (darker) { std::cout << "Darker!" << std::endl; }

            RenderEvent e(mCells[x][y], x, y, darker);
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
    
    //ConnectPoints();

}

void Level::PlaceEntity(Entity *e)
{
    while (mCells[e->cPhysics->x][e->cPhysics->y] == nullptr || mCells[e->cPhysics->x][e->cPhysics->y]->BlocksMovement())
    {
        e->cPhysics->x = rand() % MAP_WIDTH;
        e->cPhysics->y = rand() % MAP_HEIGHT;
    }
}

void Level::FromLevelConfig(LevelConfig config)
{
    ResetFOV();
    Entity *tileObjects[255];

    Entity *w = new Entity("unbreakable_wall");
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            mCells[x][y] = w;
        }
    }

    for (int i = 0; i < 255; ++i)
    {
        if (config.tileTypes[i] != "\0")
        {
            tileObjects[i] = new Entity(config.tileTypes[i]);
        }
        else
        {
            tileObjects[i] = nullptr;
        }
    }

    for (int x = 0; x < config.width; ++x)
    {
        for (int y = 0; y < config.height; ++y)
        {
            //delete mCells[x][y];
            if (tileObjects[config.tiles[x][y]] == nullptr)
            {
                mCells[x][y] = (tileObjects[1])->Clone();
                mCells[x][y]->cRender->glyph = config.tiles[x][y];
            }
            else
            {
                mCells[x][y] = (tileObjects[config.tiles[x][y]])->Clone();
            }
            
            mCells[x][y]->SetXY(x, y);
        }
    }

    for (int i = 0; i < 255; ++i)
    {
        delete tileObjects[i];
    }
}

byte Level::GetFOV(int x, int y)
{
    return fovMap[x][y];
}

byte Level::GetFOV(point p)
{
    return GetFOV(p.first, p.second);
}

void Level::SetFOV(int value, int x, int y)
{
    fovMap[x][y] = (byte)(value);
}

void Level::RevealAll()
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            fovMap[x][y] = fovVisible;
        }
    }
}
