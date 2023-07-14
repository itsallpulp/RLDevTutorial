#pragma once

#include "Entity.h"
#include "Types.h"
#include "Globals.h"
#include "Event.h"
#include "FOVData.h"
#include "LevelConfig.h"


class Level
{
	private:
	Entity *mTiles[255];
	Entity *mCells[MAP_WIDTH][MAP_HEIGHT];
	int mRegions[MAP_WIDTH][MAP_HEIGHT];
	byte fovMap[MAP_WIDTH][MAP_HEIGHT];
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

	void ResetFOV();

	void ConnectPoints();

	public:
	Level();
	Level(json::object toLoad);
	~Level();
	void ClearCells();
	json::object ToJson();
	void LoadJson(json::object data);
	void Render(int xOff = 0, int yOff = 0);
	Entity* GetCell(int x, int y);

	void ConnectRegion();

	void RoomsAndMazes(int roomPlacementAttempts = 1000);
	void PlaceEntity(Entity *e);

	void FromLevelConfig(LevelConfig config);

	byte GetFOV(int x, int y);
	byte GetFOV(point p);
	void SetFOV(int value, int x, int y);
	void RevealAll();
};