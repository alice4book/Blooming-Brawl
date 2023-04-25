#pragma once
#include "Component.h"
//WIP
class DynamicColliderComponent;

class PickUp : public Component
{
private:
	DynamicColliderComponent* colliderBody;

public:
	PickUp(Entity* parent, DynamicColliderComponent* collider);
	~PickUp(){};
	void use();
};


