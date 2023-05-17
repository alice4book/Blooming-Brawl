#pragma once
#include "Component.h"
#include "glm/glm.hpp"
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

	TimeManager* timeManager;

	glm::vec3 rimColor;

public:
	PickUp(Entity* parent, Spawner* spawner,
		DynamicColliderComponent* collider, glm::vec3 color);

	void use();
};


