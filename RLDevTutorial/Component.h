#pragma once

#include "Types.h"

class Component
{
	public:
	Component() {};
	virtual void LoadJson(json::object data) {};
};