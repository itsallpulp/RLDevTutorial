#pragma once
#include "Component.h"
class RenderComponent : public Component
{
	public:
	RenderComponent(int glyph = 1, char color = 'w', char bgColor = 'x');
	int glyph;
	char color, bgColor;

	void LoadJson(json::object data);
};