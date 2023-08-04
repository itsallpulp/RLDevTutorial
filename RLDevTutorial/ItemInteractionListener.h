#pragma once
#include "Listener.h"
class ItemInteractionListener : public Listener
{
	private:
	int FireGrabItemEvent(GrabItemEvent *e);
	int FireConsumeItemEvent(ConsumeItemEvent *e);
	int FireDropItemEvent(DropItemEvent *e);


	public:
	ItemInteractionListener();
};