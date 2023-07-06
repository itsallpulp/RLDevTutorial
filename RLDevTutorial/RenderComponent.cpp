#include "RenderComponent.h"

RenderComponent::RenderComponent(int glyph, char color, char bgColor)
{
	this->glyph = glyph;
	this->color = color;
	this->bgColor = bgColor;
}

void RenderComponent::LoadJson(json::object data)
{
	glyph = data.contains("glyph") ? json::value_to<int>(data["glyph"]) : glyph;
	color = data.contains("color") ? json::value_to<std::string>(data["color"]).at(0) : color;
	bgColor = data.contains("bgColor") ? json::value_to<std::string>(data["bgColor"]).at(0) : bgColor;
}
