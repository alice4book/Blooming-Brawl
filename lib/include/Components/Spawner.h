#pragma once
#include "Component.h"
#include "Entity.h"

class PickUp;
class Shader;
class TimeManager;
class DynamicColliderComponent;
class World;
class Entity;

class Spawner : public Component {
	
private:

	World* world;

	Shader* shader;

	Entity spawndItem;

	TimeManager* timeManager;

	float timeToSpawn;

	float timer;

	bool isSpawned;

public:
	Spawner(Entity* parent, Shader* shader);

	void update() override;

	void createPickUp();
};