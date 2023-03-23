#include "StaticColliderComponent.h"

StaticColliderComponent::StaticColliderComponent(glm::vec2 center, glm::vec2 size, bool isPassable)
: center(center), size(size), isPassable(isPassable){
    compType = eCollisionStatic;
}