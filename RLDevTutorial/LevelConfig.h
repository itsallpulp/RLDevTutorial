#pragma once
#include "Types.h"

struct LevelConfig {
	int width, height;
	std::string decorationFile;
	std::string tileTypes[255];
	std::vector<std::vector<int>> tiles;
};