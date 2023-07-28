#pragma once
#include <string>
#include <vector>

#include "Types.h"

const int
FT_SLOW = 8,
FT_REG = 5,
FT_FAST = 3;

typedef struct FloatingText {
	std::string msg;
	int x, y, opacity, ticks, speed, glyph;
	char color;
};

extern std::vector<FloatingText> *floatingTexts;


extern void AddFloatingText(std::string text, char color, int x, int y, int speed = FT_REG);
extern void AddFloatingText(std::string text, char color, point p, int speed = FT_REG);
extern void AddFloatingText(int glyph, char color, point p, int speed = FT_REG);