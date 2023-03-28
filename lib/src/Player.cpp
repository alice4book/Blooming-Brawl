#include "Player.h"

Player::Player(const std::string& path, Shader* s, EPlayerID playerID)
	: Entity(path, s)
{
	ID = playerID;
}

EPlayerID Player::getID()
{
	return ID;
}

float Player::getSpeed()
{
	return speed;
}

void Player::setSpeed(int newSpeed)
{
	speed = newSpeed;
}
