#include "Entity.h"
#include "Transform.h"
#include "StaticColliderComponent.h"
#include "DynamicColliderComponent.h"

DynamicColliderComponent::DynamicColliderComponent(Entity *parent, float radius)
: Component(parent), radius(radius) {
    compType = eCollisionDynamic;
    updateCollidersList();
}

void DynamicColliderComponent::update() {
    Component::update();

    checkAllCollisions();
}

void DynamicColliderComponent::updateCollidersList() {
    // parent has to be a skybox Entity
    for(Entity* child : parent->getParent()->getChildren()){
        auto* statComp = (StaticColliderComponent*)child->getComponentByType(eCollisionStatic);
        if(statComp != nullptr){
            staticColliders.push_back(statComp);
        }
    }
}

void DynamicColliderComponent::checkAllCollisions(){
    glm::vec3 circlePosition3d = parent->transform->getLocalPosition();
    glm::vec2 circlePosition = {circlePosition3d.x, circlePosition3d.z};

    for(StaticColliderComponent* statComp : staticColliders){
         //check if two colliders fade over
         glm::vec2 collisionDirection = checkCollisionDirection(statComp->getSize(), statComp->getCenter(), circlePosition);
        if(collisionDirection.x + collisionDirection.y != 0){
            if(!statComp->getIsPassable()){
                parent->transform->addToLocalPosition({collisionDirection.x, 0, collisionDirection.y});
            }
        }
    }
}

glm::vec2 DynamicColliderComponent::checkCollisionDirection(glm::vec2 squareSize, glm::vec2 squarePosition, glm::vec2 circlePosition) {
    glm::vec2 distance;

    distance.x = circlePosition.x - squarePosition.x;
    distance.y = circlePosition.y - squarePosition.y;

    if (abs(distance.x) > (squareSize.x/2 + radius))
        return {0,0};
    if (abs(distance.y) > (squareSize.y/2 + radius))
        return {0, 0};

    if (abs(distance.x) <= (squareSize.x/2)) {
        if (distance.y >= 0) {
            return {0, (squareSize.y / 2 + radius) - distance.y};
        } else {
            return {0, -((squareSize.y / 2 + radius) + distance.y)};
        }
    }
    if (abs(distance.y) <= (squareSize.y/2)) {
        if (distance.x >= 0) {
            return { (squareSize.x / 2 + radius) - distance.x, 0};
        } else {
            return {-((squareSize.x / 2 + radius) + distance.x), 0};
        }
    }

    float cornerDistance = (distance.x - squareSize.x / 2) + (distance.y - squareSize.y/2);

    if(cornerDistance * cornerDistance <= (radius * radius))
        return {distance.x - squareSize.x/2 + radius,distance.y - squareSize.y/2 + radius};
    else
        return {0, 0};
}




