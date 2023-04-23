#pragma once
#include "Component.h"
//WIP
class DynamicColliderComponent;

class PickUp : Component
{
private:
	DynamicColliderComponent* colliderBody;

public:
	PickUp(Entity* parent, DynamicColliderComponent* collider);
};


