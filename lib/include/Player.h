#pragma once
#include "Enums.h"
#include "Component.h"

class Entity;

class Player : public Component {

private:
	EPlayerID ID;
	float speed = 1.0f;
public:
	Player(Entity* parent, EPlayerID playerID);

	EPlayerID getID();
	float getSpeed();
	void setSpeed(int newSpeed);
};