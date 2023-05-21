#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include "Enums/PickupType.h";

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

	bool isSpawned;

	glm::vec3 rimColor;

public:

	Spawner(Entity* parent, Shader* shader);

	void update() override;

	Entity* createPickUp(glm::vec3 color, EPickUp type = EPickUp::NoPower);

	Entity* drawPickUp();

	//spawner dosent start spawning again until pickup powerup is finished
	void disablePickUp();
};