#pragma once
#include "Component.h"
#include "glm/glm.hpp"
//WIP
class DynamicColliderComponent;
class Spawner;

class PickUp : public Component
{
private:
	DynamicColliderComponent* colliderBody;
	Spawner* spawn;

public:
	PickUp(Entity* parent, Spawner* spawner,
		DynamicColliderComponent* collider, glm::vec3 color);
	void use();
	glm::vec3 rimColor;
};


