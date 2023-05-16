#include "PickUp.h"
#include "glm/glm.hpp"
#include "DynamicColliderComponent.h"
#include "Spawner.h"
#include <iostream>

PickUp::PickUp(Entity* parent, Spawner* spawner, DynamicColliderComponent* collider, glm::vec3 color)
	: Component(parent)
	, colliderBody(collider)
	, spawn(spawner)
	, rimColor(color)
{}

void PickUp::use()
{ 
	spawn->disablePickUp();
}
