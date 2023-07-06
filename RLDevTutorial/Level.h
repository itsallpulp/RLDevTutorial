#pragma once

#include "Entity.h"
#include "Types.h"
#include "Globals.h"
#include "Event.h"


class Level
{
	private:
	Entity *mCells[MAP_WIDTH][MAP_HEIGHT];
	int mRegions[MAP_WIDTH][MAP_HEIGHT];
	int nRegions;
	std::vector<point> GetPossibleDoors(int region);
	bool IsPossibleDoor(int x, int y, int dx, int dy);
	void RemoveDeadEnd(int x, int y, Entity *t);
	int DiamondCountFloors(int x, int y);

	bool RegionsAssimilated();
	void FloodfillRegion(int x, int y);

	void SetCell(Entity *t, int x, int y);

	bool AddRandomRoom();
	void AddRoom(point topLeft, point bottomRight);
	void CarveMaze(int x, int y, Entity *tile);
	int CountSurroundingFloors(int x, int y);

	bool MazeCanGo(int x, int y, int dx, int dy);

	void Fill(Entity *t);

	public:
	Level();
	Level(json::object toLoad);
	json::object ToJson();
	void LoadJson(json::object data);
	void Render(int xOff = 0, int yOff = 0);
	Entity* GetCell(int x, int y);

	void ConnectRegion();

	void RoomsAndMazes();
};