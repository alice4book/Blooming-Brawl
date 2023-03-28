#pragma once
#include "Entity.h"

class Player : public Entity {

private:
	int ID;
	float speed = 0.01;
public:
	Player(const std::string& path, Shader* s, int playerID);

	int getID();
	float getSpeed();

};