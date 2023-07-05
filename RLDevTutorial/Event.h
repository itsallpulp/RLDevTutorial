#pragma once

class Entity;

static enum EventID {
	evMove = 1,
	evRender = 2,
	NUM_EVENTS
};

class Event {
	public:
	Event(int t)
	{
		type = t;
	}

	int GetType()
	{
		return type;
	}

	protected:
	int type;
};

class MovementEvent : public Event {
	public:
	MovementEvent(Entity *t, int x, int y) : Event(evMove)
	{
		target = t;
		dx = x;
		dy = y;
	}

	Entity *target;
	int dx, dy;
};

class RenderEvent : public Event {
	public:
	RenderEvent(Entity *t) : Event(evRender)
	{
		target = t;
	}
	Entity *target;

};