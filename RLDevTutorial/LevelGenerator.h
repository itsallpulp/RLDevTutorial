#pragma once

#include "LevelConfig.h"

class LevelGenerator
{
	public:
	LevelGenerator();
	virtual LevelConfig GenerateLevel();
};