#pragma once
#include "Listener.h"

#include "Globals.h"
#include "FOVData.h"
#include "Level.h"

class FOVListener : public Listener
{
	private:
	int FireMovementEvent(MovementEvent *e);
	bool CanSee(int x1, int y1, int x2, int y2);
	void CastLight(uint x, uint y, uint radius, uint row, float startSlope, float endSlope, uint xx, uint xy, uint yx, uint yy);

	public:
	FOVListener();
};

