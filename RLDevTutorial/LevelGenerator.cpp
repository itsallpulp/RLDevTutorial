#include "LevelGenerator.h"
#include "Globals.h"

void LevelGenerator::CarveMaze(int x, int y, int value)
{
    config.tiles[x][y] = value;

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
        if (MazeCanGo(x, y, (*it).first, (*it).second, 0))
        {
            config.tiles[x + (*it).first][y + (*it).second] = value;
            //mRegions[x + (*it).first][y + (*it).second] = nRegions;
            CarveMaze(x + (*it).first * 2, y + (*it).second * 2, value);
        }
        directions.erase(it);
    }
}

bool LevelGenerator::MazeCanGo(int x, int y, int dx, int dy, int goodVal)
{
    if (x + dx * 2 >= width - 1) { return false; }
    if (x + dx * 2 <= 0) { return false; }
    if (y + dy * 2 >= height) { return false; }
    if (y + dy * 2 <= 0) { return false; }

    return (config.tiles[x + (dx * 2)][y + (dy * 2)] == goodVal);
}

bool LevelGenerator::AddRandomRoom(int floorVal)
{
    int x = (rand() % width / 2) * 2;
    if (x % 2 != 0) { ++x; }
    int y = (rand() % height / 2) * 2;
    if (y % 2 != 0) { ++y; }
    int rWidth = (rand() % 6) * 2 + 7,
        rHeight = (rand() % 4) * 2 + 7;

    if (x + rWidth >= width || y + rHeight >= height) { return false; }

    for (int i = x; i < x + rWidth; ++i)
    {
        for (int j = y; j < y + rHeight; ++j)
        {
            if (config.tiles[i][j] != 0)
            {
                return false;
            }
        }
    }

    AddRoom({ x, y }, { x + rWidth, y + rHeight }, floorVal);

    return true;
}



void LevelGenerator::AddRoom(point topLeft, point bottomRight, int floorVal)
{
    for (int x = topLeft.first + 1; x < bottomRight.first - 1; ++x)
    {
        for (int y = topLeft.second + 1; y < bottomRight.second - 1; ++y)
        {
            config.tiles[x][y] = floorVal;
        }
    }
}

int LevelGenerator::CountSurroundingTiles(int x, int y, int value)
{
    int n = 0;

    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            if (i == 0 && j == 0) { continue; }
            if (config.tiles[x + i][y + j] == value)
            {
                ++n;
            }
        }
    }

    return n;
}

std::vector<std::vector<int>> LevelGenerator::GenerateRegionMap()
{
    std::vector<std::vector<int>> regions;
    for (int x = 0; x < width; ++x)
    {
        regions.push_back(std::vector<int>());
        for (int y = 0; y < height; ++y)
        {
            regions[x].push_back(-1);
        }
    }

    int r = 0;
    for (int x = 1; x < width-1; ++x)
    {
        for (int y = 1; y < height - 1; ++y)
        {
            if (regions[x][y] == -1 && config.tiles[x][y] != 0)
            {
                FloodFill(&regions, x, y, r++);
            }
        }
    }

    std::cout << r << " regions" << std::endl;

    return regions;
}

void LevelGenerator::FloodFill(std::vector<std::vector<int>> *m, int x, int y, int value)
{
    m->at(x).at(y) = value;

    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            if (i == 0 && j == 0) { continue; }

            if (config.tiles[x+i][y+j] != 0 && m->at(x+i).at(y+j) != value)
            {
                FloodFill(m, x + i, y + j, value);
            }
        }
    }
}

std::vector<point> LevelGenerator::GetPossibleDoors(const std::vector<std::vector<int>> regionMap, int regionVal)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Searching for doors...\t";
    std::vector<point> doors;
    for (int x = 1; x < width - 1; ++x)
    {
        for (int y = 1; y < height - 1; ++y)
        {
            //std::cout << mRegions[x][y] << ", ";
            if (regionMap[x][y] != regionVal) { continue; }

            auto s1 = std::chrono::high_resolution_clock::now();

            if (IsPossibleDoor(regionMap, x, y, 1, 0)) { doors.push_back({ x + 1, y }); }
            if (IsPossibleDoor(regionMap, x, y, -1, 0)) { doors.push_back({ x - 1, y }); }
            if (IsPossibleDoor(regionMap, x, y, 0, 1)) { doors.push_back({ x, y + 1 }); }
            if (IsPossibleDoor(regionMap, x, y, 0, -1)) { doors.push_back({ x, y - 1 }); }
            auto s2 = std::chrono::high_resolution_clock::now();

            std::cout << "Checked " << x << ", " << y << " to be a door in " << std::chrono::duration_cast<std::chrono::microseconds>(s2 - s1).count() << "ms" << std::endl;

        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    long long total = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << "Found " << doors.size() << " possible doors in " << total << "ms" << std::endl;

    return doors;
}

bool LevelGenerator::IsPossibleDoor(const std::vector<std::vector<int>> regionMap, int x, int y, int dx, int dy)
{
    if (x + dx * 2 <= 0 || x + dx * 2 >= width - 1) { return false; }
    if (y + dy * 2 <= 0 || y + dy * 2 >= height - 1) { return false; }

    int x1 = x + dx, y1 = y + dy;
    int x2 = x + (dx * 2), y2 = y + (dy * 2);

    return(
        (config.tiles[x1][y1] == 0 && config.tiles[x2][y2] != 0) && // The tile between the two is a wall
        (regionMap[x][y] != regionMap[x2][y2]) // The tiles on opposite sides of the wall are unequal
        );
        
        /*(
        (mRegions[x + dx][y + dy] == -1 && !mRegions[x + dx * 2][y + dy * 2] == -1) &&
        (mRegions[x][y] != mRegions[x + dx * 2][y + dy * 2])
        );*/
}

bool LevelGenerator::AllRegionsAssimilated(const std::vector<std::vector<int>> regionMap)
{
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            if (regionMap[x][y] > 0)
            {
                return false;
            }
        }
    }

    return true;
}



LevelGenerator::LevelGenerator(int width, int height)
{
    //std::cout << width << "x" << height << std::endl;
    this->width = width;
    this->height = height;

    config.width = width;
    config.height = height;

    for (int i = 0; i < 255; ++i)
    {
        config.tileTypes[i] = "\0";
    }

    config.tileTypes[0] = "base_wall";
    config.tileTypes[1] = "base_floor";
    config.tileTypes[2] = "base_door";

    for (int x = 0; x < width; ++x)
    {
        config.tiles.push_back(std::vector<int>());
        for (int y = 0; y < height; ++y)
        {
            config.tiles[x].push_back(0);
        }
    }
}

LevelConfig LevelGenerator::GenerateLevel()
{
    return LevelConfig();
}
