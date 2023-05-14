#include "PickUp.h"
#include "glm/glm.hpp"
#include "DynamicColliderComponent.h"
#include <iostream>

PickUp::PickUp(Entity* parent, DynamicColliderComponent* collider):
	Component(parent)
	,colliderBody(collider)
{}

void PickUp::use()
{
	//delete parent;
	//std::cout << " SSS " << std::endl;
}
