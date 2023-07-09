#pragma once
#include "Component.h"
class FOVComponent : public Component
{
	public:
	FOVComponent(int distance = 100);
	int viewDistance;
	void LoadJson(json::object data);
};