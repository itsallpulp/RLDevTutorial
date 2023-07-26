#pragma once
#include <string>

const int
FT_SLOW = 8,
FT_REG = 5,
FT_FAST = 3;

typedef struct FloatingText {
	std::string msg;
	int x, y, opacity, ticks, speed;
	char color;
};