#pragma once
#include "Listener.h"
class LogListener : public Listener
{
	private:
	int FireMovementEvent(MovementEvent *e);
	int FireLogEvent(LogEvent *e);

    public:
    LogListener();
};

