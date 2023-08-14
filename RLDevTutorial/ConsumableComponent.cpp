#include "ConsumableComponent.h"

ConsumableComponent::ConsumableComponent()
{
	healAmount = 0;
	verb = "eat";
	targetType = "NA";
}

void ConsumableComponent::LoadJson(json::object data)
{
	healAmount = data.contains("healAmount") ? json::value_to<int>(data["healAmount"]) : healAmount;
	verb = data.contains("verb") ? json::value_to<std::string>(data["verb"]) : verb;

	if (data.contains("zaps"))
	{
		targetType = json::value_to<std::string>(data["zaps"].as_object()["targetType"]);

		for (json::value v : data["zaps"].as_object()["effects"].as_array())
		{
			LoadZap(v.as_object());
		}
	}
}

void ConsumableComponent::LoadZap(json::object data)
{
	Zap z;
	z.amount = data.contains("amount") ? json::value_to<int>(data["amount"]) : 0;
	z.type = data.contains("type") ? json::value_to<std::string>(data["type"]) : "None";
	z.subtype = data.contains("subtype") ? json::value_to<std::string>(data["subtype"]) : "None";

	zaps.push_back(z);
}
