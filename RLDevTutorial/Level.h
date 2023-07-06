#pragma once

#include "Entity.h"
#include "Types.h"
#include "Globals.h"
#include "Event.h"

class Level
{
	private:
	Entity *mCells[MAP_WIDTH][MAP_HEIGHT];

	public:
	Level();
	Level(json::object toLoad);
	json::object ToJson();
	void LoadJson(json::object data);
	void Render(int xOff = 0, int yOff = 0);
	Entity* GetCell(int x, int y);
};