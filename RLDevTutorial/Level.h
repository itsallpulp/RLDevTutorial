#pragma once

#include "Entity.h"
#include "Types.h"
#include "Globals.h"
#include "Event.h"

class Level
{
	private:
	Entity *mCells[MAP_WIDTH][MAP_HEIGHT];

	void SetCell(Entity *t, int x, int y);

	bool AddRandomRoom();
	void AddRoom(point topLeft, point bottomRight);
	void StartMaze(int x, int y, Entity *tile);

	void Fill(Entity *t);

	public:
	Level();
	Level(json::object toLoad);
	json::object ToJson();
	void LoadJson(json::object data);
	void Render(int xOff = 0, int yOff = 0);
	Entity* GetCell(int x, int y);

	void RoomsAndMazes();
};