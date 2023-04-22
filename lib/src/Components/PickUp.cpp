#include "PickUp.h"
#include "glm/glm.hpp"
#include "DynamicColliderComponent.h"

PickUp::PickUp(Entity* parent, DynamicColliderComponent* collider):
	Component(parent)
	,colliderBody(collider)
{}

void PickUp::update() {};

void PickUp::collisionDetection()
{
    
    if (colliderBody->getTouchingComponents().empty())
        return;

    for (auto comp : colliderBody->getTouchingComponents()) {

        if (dynamic_cast<DynamicColliderComponent*>(comp) != nullptr) {
            
        }
    }
}