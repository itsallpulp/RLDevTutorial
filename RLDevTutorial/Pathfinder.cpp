#include "Pathfinder.h"

Pathfinder::Pathfinder()
{
}

std::vector<point> Pathfinder::GetNeighbors(point p)
{
	std::vector<point> neighbors;
	
	if (!level->GetCell(p.first+1, p.second)->BlocksMovement()) { neighbors.push_back({ p.first+1, p.second}); }
	if (!level->GetCell(p.first-1, p.second)->BlocksMovement()) { neighbors.push_back({ p.first-1, p.second }); }
	if (!level->GetCell(p.first, p.second+1)->BlocksMovement()) { neighbors.push_back({ p.first, p.second+1}); }
	if (!level->GetCell(p.first, p.second-1)->BlocksMovement()) { neighbors.push_back({ p.first, p.second-1}); }
	
	return neighbors;
}

std::stack<point> Pathfinder::ReconstructPath(std::map<point, point> cameFrom, point current)
{
	std::stack<point> path;
	while (cameFrom.count(current) != 0)
	{
		path.push({ current.first, current.second });
		current = cameFrom[current];
	}
	return path;
}

std::stack<point> Pathfinder::GeneratePath(point start, point goal)
{
	std::set<point> openSet;
	std::map<point, point> cameFrom;
	std::map<point, int> gScore;
	std::map<point, double> fScore;

	openSet.insert(start);
	gScore[start] = 0;
	fScore[start] = Distance(start, goal);

	while (!openSet.empty())
	{
		point current = *(openSet.begin());
		double cost = fScore[current];

		for (point c : openSet)
		{
			if (fScore[c] < cost)
			{
				cost = fScore[c];
				current = c;
			}
		}

		if (current.first == goal.first && current.second == goal.second)
		{
			return ReconstructPath(cameFrom, current);
		}

		openSet.erase(current);

		for (point n : GetNeighbors(current))
		{
			int gTent = gScore[current] + 1;
			if (n.first != goal.first || n.second != goal.second)
			{
				if ( level->GetCell(n.first, n.second)->BlocksMovement()) { gTent += MAP_WIDTH * MAP_HEIGHT; }
				//if (Engine::Zone::currentZone->flags[n.first][n.second] & cBlockMovement) { gTent += wallCost; }
				//if (cellFlags[n.first][n.second] & CFActor) { gTent += CELL_ACTOR_COST; }
			}

			double fTent = gTent + Distance(n, goal);
			/*
			if (gScore.count(n) == 0)
			{
				std::cout << "Haven't seen cell " << n.first << ", " << n.second << " before" << std::endl;
			}
			*/
			if (gScore.count(n) == 0 || gTent < gScore[n])
			{
				cameFrom[n] = current;
				gScore[n] = gTent;
				fScore[n] = fTent;
				if (openSet.find(n) == openSet.end()) { openSet.insert(n); }
			}
		}
	}

	return std::stack<point>();
}

int** Pathfinder::CreateDijkstraMap(std::vector<point> goals)
{
	int **m = 0;
	m = new int*[MAP_WIDTH];

	
	for (int x = 0; x < MAP_WIDTH; ++x)
	{
		m[x] = new int[MAP_HEIGHT];
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			m[x][y] = MAP_WIDTH * MAP_HEIGHT;
		}
	}

	std::queue<point> unvisited;

	for (point p : goals)
	{
		m[p.first][p.second] = 0;

		unvisited.push(p);
	}

	while (!unvisited.empty())
	{
		point current = unvisited.front();
		unvisited.pop();

		int value = m[current.first][current.second];
		std::vector<point> neighbors = GetNeighbors(current);

		for (point n : neighbors)
		{
			if (m[n.first][n.second] > value + 1)
			{
				m[n.first][n.second] = value + 1;
				unvisited.push(n);
			}
		}
		neighbors.clear();
	}

	return m;
}
