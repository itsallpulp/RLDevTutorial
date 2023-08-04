#pragma once
#include "Listener.h"
class ItemInteractionListener : public Listener
{
	private:
	int FireGrabItemEvent(GrabItemEvent *e);
	int FireConsumeItemEvent(ConsumeItemEvent *e);


	public:
	ItemInteractionListener();
};