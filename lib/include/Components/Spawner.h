#pragma once
#include "Component.h"
#include <vector>
#include <memory>
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

	Entity* currentItem;

	TimeManager* timeManager;

	std::vector<Entity*> pickUps;

	float timeToSpawn;

	float timer;


public:

	Spawner(Entity* parent, Shader* shader);

	bool isSpawned;

	void update() override;

	Entity* createPickUp(glm::vec3 color);

	Entity* drawPickUp();

	void disablePickUp();
};