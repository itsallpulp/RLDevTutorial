#include "FOVComponent.h"

FOVComponent::FOVComponent(int distance)
{
	viewDistance = distance;
}

void FOVComponent::LoadJson(json::object data)
{
	viewDistance = data.contains("distance") ? json::value_to<int>(data["distance"]) : viewDistance;
}
