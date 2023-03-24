#include "DynamicColliderComponent.h"

DynamicColliderComponent::DynamicColliderComponent(Entity *parent, glm::vec2 center, float radius)
: Component(parent), center(center), radius(radius) {
    compType = eCollisionDynamic;
//    parent->addChild();
}

void DynamicColliderComponent::CollisionCheck(){
    for(StaticColliderComponent* singleObj : staticColliders){
         //check if two colliders fade over
    }
}