#include "FOVListener.h"

int FOVListener::FireMovementEvent(MovementEvent *e)
{
	DoFOV(e->target);
    return 0;
}

bool FOVListener::CanSee(int x1, int y1, int x2, int y2)
{
	// http://www.roguebasin.com/index.php/Bresenham%27s_Line_Algorithm
	int deltaX = x2 - x1;
	signed char const ix = ((deltaX > 0) - (deltaX < 0));
	deltaX = std::abs(deltaX) << 1;

	int deltaY = y2 - y1;
	signed char const iy = ((deltaY > 0) - (deltaY < 0));
	deltaY = std::abs(deltaY) << 1;

	int maxMonsterSight = 15;

	int X2 = x2,
		X1 = x1,
		Y2 = y2,
		Y1 = y1,
		count = 0;

	if (deltaX >= deltaY)
	{
		int error = deltaY - (deltaX >> 1);


		while (X1 != X2)
		{
			if ((error > 0) || (!error && (ix > 0)))
			{
				error -= deltaX;
				Y1 += iy;
			}

			error += deltaY;
			X1 += ix;
			++count;

			if (level->GetCell(X1, Y1)->cPhysics->blocksVision || count > 25) { return false; }
		}
	}
	else
	{
		int error = deltaX - (deltaY >> 1);

		while (Y1 != Y2)
		{
			if ((error > 0) || (!error && (iy > 0)))
			{
				error -= deltaY;
				X1 += ix;
			}

			error += deltaX;
			Y1 += iy;
			++count;

			if (level->GetCell(X1, Y1)->cPhysics->blocksVision || count > 25) { return false; }
		}
	}

	return true;
}

void FOVListener::CastLight(uint x, uint y, uint radius, uint row, float startSlope, float endSlope, uint xx, uint xy, uint yx, uint yy)
{
	if (startSlope < endSlope) {
		return;
	}
	float next_startSlope = startSlope;
	for (uint i = row; i <= radius; i++)
	{
		bool blocked = false;
		for (int dx = -i, dy = -i; dx <= 0; dx++) // Needed to add "/wd4146" to Properties > Configuration Properties > C/C++ > All Options / Additional Options
		{
			float l_slope = (dx - 0.5) / (dy + 0.5);
			float r_slope = (dx + 0.5) / (dy - 0.5);

			if (startSlope < r_slope)
			{
				continue;
			}
			else if (endSlope > l_slope)
			{
				break;
			}

			int sax = dx * xx + dy * xy;
			int say = dx * yx + dy * yy;

			if ((sax < 0 && (uint)std::abs(sax) > x) || (say < 0 && (uint)std::abs(say) > y))
			{
				continue;
			}

			uint ax = x + sax;
			uint ay = y + say;

			if (ax >= MAP_WIDTH || ay >= MAP_HEIGHT)
			{
				continue;
			}

			uint radius2 = radius * radius;
			if ((uint)(dx * dx + dy * dy) < radius2)
			{
				//std::cout << "\tVisible: " << ax << ", " << ay << std::endl;
				//cellFlags[ax][ay] |= FOVVisible | FOVMemory;
				//cellFlags[ax][ay] &= ~(FOVHidden);
				level->SetFOV(fovVisible, ax, ay);
			}

			if (blocked)
			{
				if (level->GetCell(ax, ay)->cPhysics->blocksVision)
				{
					next_startSlope = r_slope;
					continue;
				}
				else
				{
					blocked = false;
					startSlope = next_startSlope;
				}
			}
			else if (level->GetCell(ax, ay)->cPhysics->blocksVision)
			{
				blocked = true;
				next_startSlope = r_slope;
				CastLight(x, y, radius, i + 1, startSlope, l_slope, xx, xy, yx, yy);
			}
		}
		if (blocked)
		{
			break;
		}
	}
}

FOVListener::FOVListener()
{
    RegisterListenFor(evMove);
}

void FOVListener::DoFOV(Entity *target)
{
	if (target->cFOV == nullptr) { return; }

	int tx = target->cPhysics->x,
		ty = target->cPhysics->y;

	for (int x = 0; x < MAP_WIDTH; ++x)
	{
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			if (level->GetFOV(x, y) == fovVisible)
			{
				//std::cout << "Set to memory" << std::endl;
				level->SetFOV(fovMemory, x, y);
			}
		}
	}

	level->SetFOV(fovVisible, tx, ty);

	for (int i = 0; i < 8; ++i)
	{
		CastLight(tx, ty, target->cFOV->viewDistance, 1, 1.0, 0.0, fovMultipliers[0][i], fovMultipliers[1][i], fovMultipliers[2][i], fovMultipliers[3][i]);
	}
}
