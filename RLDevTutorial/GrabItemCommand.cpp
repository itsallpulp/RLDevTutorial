#include "GrabItemCommand.h"

#include "Entity.h"
#include "EntityManager.h"

GrabItemCommand::GrabItemCommand(Entity *target)
{
	this->target = target;
}

int GrabItemCommand::Execute()
{
	point p = target->GetXY();
	Entity *n = itemManager->At(p.first, p.second);

	if (n == nullptr)
	{
		LogEvent l(target, "There is nothing to pick up here.");
		return WorldFireEvent(&l);
	}

	GrabItemEvent g(target, n);

	return WorldFireEvent(&g);
}
