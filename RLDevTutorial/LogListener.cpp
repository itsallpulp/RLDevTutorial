#include "LogListener.h"

int LogListener::FireMovementEvent(MovementEvent *e)
{
	e->target->SendLog("You moved");
	return 0;
}

int LogListener::FireLogEvent(LogEvent *e)
{
	e->target->SendLog(e->msg);
	return 0;
}

LogListener::LogListener()
{
	RegisterListenFor(evLog);
}
