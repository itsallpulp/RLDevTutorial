#pragma once
#include "LevelGenerator.h"
class LGNystromRoomsAndMazes : public LevelGenerator
{
	public:
	LGNystromRoomsAndMazes(int width, int height);
	LevelConfig GenerateLevel();
};