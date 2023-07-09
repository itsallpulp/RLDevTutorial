#pragma once

static int fovMultipliers[4][8] = {
			{1, 0, 0, -1, -1, 0, 0, 1},
			{0, 1, -1, 0, 0, -1, 1, 0},
			{0, 1, 1, 0, 0, -1, -1, 0},
			{1, 0, 0, 1, -1, 0, 0, -1}
};

static enum FovValues {
	fovHidden = 0,
	fovVisible,
	fovMemory
};