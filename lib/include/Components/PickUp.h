#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include "Enums/PickupType.h"

//WIP
class DynamicColliderComponent;
class Spawner;
class TimeManager;
class Shader;

class PickUp : public Component
{
private:
	DynamicColliderComponent* colliderBody;

	Spawner* spawn;

	glm::vec3 rimColor;

	float timer;

	TimeManager* timeManager;

	Entity* player;

	float pickupMovement;
public:
	PickUp(Entity* parent, Spawner* spawner,
		DynamicColliderComponent* collider, glm::vec3 color, EPickUp type = EPickUp::NoPower);

	void update() override;

	void use(Entity* player);
	void endUse();

	bool isUsed;
	bool isSpawn;
	EPickUp type;
};


