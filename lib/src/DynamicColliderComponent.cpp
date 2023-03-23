#include "DynamicColliderComponent.h"

DynamicColliderComponent::DynamicColliderComponent(glm::vec2 center, float radius)
: center(center), radius(radius){
    compType = eCollisionDynamic;
}

void DynamicColliderComponent::CollisionCheck(){
    for(StaticColliderComponent* singleObj : staticColliders){
         //check if two colliders fade over
    }
}