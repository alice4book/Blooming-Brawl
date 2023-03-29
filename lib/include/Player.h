#pragma once
#include "Enums.h"
#include "Component.h"

class Entity;

class Player : Component {

private:
	EPlayerID ID;
	float speed = 0.005;
public:
	Player(Entity* parent, EPlayerID playerID);

	EPlayerID getID();
	float getSpeed();
	void setSpeed(int newSpeed);
};