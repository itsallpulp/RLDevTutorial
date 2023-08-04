#include "ConsumableComponent.h"

ConsumableComponent::ConsumableComponent()
{
	healAmount = 0;
	verb = "eat";
}

void ConsumableComponent::LoadJson(json::object data)
{
	healAmount = data.contains("healAmount") ? json::value_to<int>(data["healAmount"]) : healAmount;
	verb = data.contains("verb") ? json::value_to<std::string>(data["verb"]) : verb;
}
