#pragma once
#include "Component.h"

class PickUp;
class Shader;
class TimeManager;


class Spawner : public Component {
	
private:
	Shader* shader;

	Entity* spawndItem;

	TimeManager* timeManager;

	float timeToSpawn;

	float timer;

public:
	Spawner(Entity* parent, Shader* shader);

	void update() override;

	Entity* spawnPickUp();

	Entity* createPickUp();
};