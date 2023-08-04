#include "HealComponent.h"

HealComponent::HealComponent()
{
	amount = 0;
	activateOnConsume = false;
}

void HealComponent::LoadJson(json::object data)
{
	amount = data.contains("amount") ? json::value_to<int>(data["amount"]) : amount;
	activateOnConsume = data.contains("activateOnConsume") ? json::value_to<bool>(data["activateOnConsume"]) : activateOnConsume;
}