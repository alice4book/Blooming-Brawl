#pragma once
#include "Entity.h"
#include "Enums.h"

class Player : public Entity {

private:
	EPlayerID ID;
	float speed = 0.005;
public:
	Player(const std::string& path, Shader* s, EPlayerID playerID);

	EPlayerID getID();
	float getSpeed();
	void setSpeed(int newSpeed);
};