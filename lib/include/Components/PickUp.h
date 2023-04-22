#pragma once
#include "Component.h"
//WIP
class DynamicColliderComponent;

class PickUp : Component
{
private:
	DynamicColliderComponent* colliderBody;

	void collisionDetection();

public:
	PickUp(Entity* parent, DynamicColliderComponent* collider);
	void update() override;
};


