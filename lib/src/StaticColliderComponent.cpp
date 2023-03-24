#include "StaticColliderComponent.h"

StaticColliderComponent::StaticColliderComponent(Entity *parent, glm::vec2 center, glm::vec2 size, bool isPassable)
: Component(parent), center(center), size(size), isPassable(isPassable) {
    compType = eCollisionStatic;
}