#pragma once

#include "Types.h"
#include "LevelConfig.h"

class LevelGenerator
{
	protected:
	int width, height;
	void CarveMaze(int x, int y, int value);
	bool MazeCanGo(int x, int y, int dx, int dy, int goodVal);
	bool AddRandomRoom(int floorVal=1);
	void AddRoom(point topLeft, point bottomRight, int floorVal);
	int CountSurroundingTiles(int x, int y, int value);
	LevelConfig config;
	std::vector<std::vector<int>> GenerateRegionMap();
	void FloodFill(std::vector<std::vector<int>> *m, int x, int y, int value);
	std::vector<point> GetPossibleDoors(const std::vector<std::vector<int>> regionMap, int regionVal);
	bool IsPossibleDoor(const std::vector<std::vector<int>> regionMap, int x, int y, int dx, int dy);
	bool AllRegionsAssimilated(const std::vector<std::vector<int>> regionMap);


	public:
	LevelGenerator(int width, int height);
	virtual LevelConfig GenerateLevel();
};