#pragma once

#include "Types.h"
#include "Globals.h"
#include "Level.h"

class Pathfinder
{
	protected:
	std::vector<point> GetNeighbors(point p);
	std::stack<point> ReconstructPath(std::map<point, point> cameFrom, point current);

	public:
	Pathfinder();
	std::stack<point> GeneratePath(point start, point goal);
};