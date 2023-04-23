#include "PickUp.h"
#include "glm/glm.hpp"
#include "DynamicColliderComponent.h"

PickUp::PickUp(Entity* parent, DynamicColliderComponent* collider):
	Component(parent)
	,colliderBody(collider)
{}
