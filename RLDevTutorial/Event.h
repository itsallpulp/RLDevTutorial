#pragma once

class Entity;

static enum EventID {
	evMove = 1,
	evRender = 2,
	evLog = 4,
	evDamage = 8,
	evAttack = 16,
	evTakeTurn = 32
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
	RenderEvent(Entity *t, int x = -1, int y = -1, bool darker=false) : Event(evRender)
	{
		target = t;
		this->x = x;
		this->y = y;
		this->darker = darker;
	}
	Entity *target;
	int x, y;
	bool darker;

};

class LogEvent : public Event {
	public:
	LogEvent(Entity *t, std::string message) : Event(evLog)
	{
		target = t;
		msg = message;
	}
	Entity *target;
	std::string msg;
};

class DamageEvent : public Event {
	public:
	DamageEvent(Entity *attacker, Entity *defender) : Event(evDamage)
	{
		this->attacker = attacker;
		this->defender = defender;
		type = "blunt";
		damage = 0;
	}
	Entity *attacker, *defender;
	int damage;
	std::string type;
};

class TurnEvent : public Event {
	public:
	TurnEvent(Entity *target) : Event(evTakeTurn)
	{
		this->target = target;
	}
	Entity *target;
};