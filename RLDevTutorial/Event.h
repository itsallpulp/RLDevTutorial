#pragma once

class Entity;

static enum EventID {
	evMove = 1,
	evRender = 2,
	evLog = 4,
	evDamage = 8,
	evAttack = 16,
	evTakeTurn = 32,
	evGrabItem = 64,
	evConsumeItem = 128,
	evDropItem = 256,
	evHeal = 512,
	evRemoveItem = 1024,
	evAddStatusEffect = 2048
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

	virtual void SetTarget(Entity *e) {};

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

		owGlyph = -1;
		owColor = '-';
		owBg = '-';
	}
	Entity *target;
	int x, y;
	bool darker;

	int owGlyph;
	char owColor, owBg;

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

	void SetTarget(Entity *e) { defender = e; }

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

class GrabItemEvent : public Event {
	public:
	GrabItemEvent(Entity *target, Entity *item) : Event(evGrabItem)
	{
		this->target = target;
		this->item = item;
	}
	Entity *target, *item;
};

class ConsumeItemEvent : public Event {
	public:
	ConsumeItemEvent(Entity *target, Entity *consumed) : Event(evConsumeItem)
	{
		this->target = target;
		this->consumed = consumed;
	}
	Entity *target, *consumed;
};

class DropItemEvent : public Event {
	public:
	DropItemEvent(Entity *target, Entity *item) : Event(evDropItem)
	{
		this->target = target;
		this->item = item;
	}
	Entity *target, *item;
};

class HealEvent : public Event {
	public:
	HealEvent(Entity *target, int amount) : Event(evHeal)
	{
		this->target = target;
		this->amount = amount;
	}
	Entity *target;
	int amount;
};

class RemoveItemEvent : public Event {
	public:
	RemoveItemEvent(Entity *holder, Entity *item) : Event(evRemoveItem)
	{
		this->holder = holder;
		this->item = item;
	}
	Entity *holder, *item;
};

class AddStatusEffectEvent : public Event {
	public:
	AddStatusEffectEvent(Entity *target, std::string effect, int rounds) : Event(evAddStatusEffect)
	{
		this->target = target;
		this->effect = effect;
		this->rounds = rounds;
	}

	void SetTarget(Entity *e)
	{
		this->target = e;
	}

	Entity *target;
	std::string effect;
	int rounds;
};