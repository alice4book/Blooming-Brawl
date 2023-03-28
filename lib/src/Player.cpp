#include "Player.h"

Player::Player(const std::string& path, Shader* s, int playerID)
	: Entity(path, s)
{
	ID = playerID;
}

int Player::getID()
{
	return ID;
}

float Player::getSpeed()
{
	return speed;
}
