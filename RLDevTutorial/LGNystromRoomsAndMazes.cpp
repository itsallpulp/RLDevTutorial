#include "LGNystromRoomsAndMazes.h"
#include "Globals.h"

LGNystromRoomsAndMazes::LGNystromRoomsAndMazes(int width, int height) : LevelGenerator(width, height)
{

}

LevelConfig LGNystromRoomsAndMazes::GenerateLevel()
{
	int roomAttempts = 100,
		extraDoorChance = 50;

	for (int i = 0; i < roomAttempts; ++i)
	{
		AddRandomRoom();
	}

	for (int x = 1; x < width-1; ++x)
	{
		for (int y = 1; y < height-1; ++y)
		{
			if(config.tiles[x][y] == 0 && CountSurroundingTiles(x,y,0) == 8)
			CarveMaze(x, y, 1);
		}
	}


	/*
	std::vector<std::vector<int>> regions = GenerateRegionMap();

	while (!AllRegionsAssimilated(regions))
	{
		std::vector<point> possibleDoors = GetPossibleDoors(regions, 0);

		auto it = possibleDoors.begin();
		std::advance(it, rand() % possibleDoors.size());
		config.tiles[(*it).first][(*it).second] = 2;
		FloodFill(&regions, (*it).first, (*it).second, 0);
	}
	*/

	return config;
}
