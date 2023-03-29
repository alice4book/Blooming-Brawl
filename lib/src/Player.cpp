#include "Player.h"

Player::Player(Entity* parent, EPlayerID playerID)
	: Component(parent)
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
