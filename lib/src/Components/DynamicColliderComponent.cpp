#include "World.h"
#include "Transform.h"
#include "StaticColliderComponent.h"
#include "DynamicColliderComponent.h"
#include "RobotMovement.h"
#include "TimeManager.h"
#include "Map.h"

DynamicColliderComponent::DynamicColliderComponent(Entity *parent, float radius, glm::vec2 centerOffset, bool isTrigger)
: Component(parent), radius(radius), centerOffset(centerOffset), isTrigger(isTrigger) {
    TimeManager::getInstance()->attachUnlimitedFPS(this);

    world = World::getInstance();
}

void DynamicColliderComponent::update() {
    checkAllCollisions();
}

void DynamicColliderComponent::checkAllCollisions(){
    glm::vec2 circlePosition = getCenter();
    setTileColliderIAmOn();
    touchingStaticComponents.clear();
    touchingDynamicComponents.clear();

    if(tileIAmOn == nullptr){
        // Map tiles
        for(StaticColliderComponent* statComp : world->getStaticColliders()){
            //check if two colliders fade over
            glm::vec2 collisionDirection = checkStaticCollisionDirection(statComp, circlePosition);
            if(collisionDirection.x + collisionDirection.y != 0) {
                if (!statComp->getIsPassable()) {
                    parent->transform->addToLocalPosition({collisionDirection.x, 0, collisionDirection.y});
                }
                touchingStaticComponents.push_back(statComp);
            }
        }
    }
    else{
        // Calculate position of tile I am on
        glm::vec2 tileImOnPosition = tileIAmOn->getCenter();
        int column = (int)(tileImOnPosition.x / 0.254f);
        int row = (int)(tileImOnPosition.y / 0.254f);

        for(int columnCounter = column - 1; columnCounter <= column + 1; columnCounter++){
            if(columnCounter < 0 || columnCounter >= 10)
                continue;
            for(int rowCounter = row - 1; rowCounter <= row + 1; rowCounter++){
                if(rowCounter < 0 || rowCounter >= 20)
                    continue;
                StaticColliderComponent* statComp = world->mapComponent->colliders[columnCounter][rowCounter];

                glm::vec2 collisionDirection = checkStaticCollisionDirection(statComp, circlePosition);
                if(collisionDirection.x + collisionDirection.y != 0) {
                    if (!statComp->getIsPassable()) {
                        parent->transform->addToLocalPosition({collisionDirection.x, 0, collisionDirection.y});
                    }
                    touchingStaticComponents.push_back(statComp);
                }
            }
        }
    }

    // Robot, enemy player
    for(DynamicColliderComponent* dynamicComp : world->getDynamicColliders()){
        if(this == dynamicComp || this->parent == dynamicComp->parent)
            continue;

        glm::vec2 colDir = checkDynamicCollisionDirection(dynamicComp, circlePosition);
        if(fabs(colDir.x) + fabs(colDir.y) == 0)
            continue;

        touchingDynamicComponents.push_back(dynamicComp);

        // your parent is not a robot, and you are not facing wall
        if(isTrigger)
            parent->transform->addToLocalPosition({colDir.x * 0.2f, 0, colDir.y * 0.2f});

    }
}

glm::vec2 DynamicColliderComponent::checkStaticCollisionDirection(StaticColliderComponent* statComp, glm::vec2 circlePosition) const {
    glm::vec2 squarePosition = statComp->getCenter();
    glm::vec2 squareSize = statComp->getSize();

    glm::vec2 distance;
    distance.x = circlePosition.x - squarePosition.x;
    distance.y = circlePosition.y - squarePosition.y;

    glm::vec2 absDistance;
    absDistance.x = fabs(distance.x);
    absDistance.y = fabs(distance.y);

    squareSize = squareSize * 0.5f;

    glm::vec2 sqrAndRad;
    sqrAndRad.x = squareSize.x + radius;
    sqrAndRad.y = squareSize.y + radius;

    if (absDistance.x > sqrAndRad.x)
        return {0,0};
    if (absDistance.y > sqrAndRad.y)
        return {0, 0};

    if (absDistance.x <= squareSize.x) {
        // (depending on distance sign change sign of sqrAndRad) - distance.y
        return {0, ((float)signbit(distance.y) * -2 * sqrAndRad.y + sqrAndRad.y) - distance.y};
    }
    if (absDistance.y <= squareSize.y) {
        // (depending on distance sign change sign of sqrAndRad) - distance.x
        return {((float)signbit(distance.x) * -2 * sqrAndRad.x + sqrAndRad.x) - distance.x, 0};
    }

    // Distance between circle and square, at which there is collision, when at corner
    // multiplied by 110% for smoother collider
    float maxCornerDistance = (radius + std::max(squareSize.x, squareSize.y)) * 1.1f;

    // A^2 + B^2 == C^2 -> if at range of collider
    if((distance.x * distance.x) + (distance.y * distance.y) < maxCornerDistance * maxCornerDistance){
        glm::vec2 norVec = circlePosition - squarePosition;
        norVec = glm::normalize(norVec);

        // Normalised distance vector * (float) distance between circles
        return norVec * (maxCornerDistance - sqrtf(distance.x * distance.x + distance.y * distance.y));
    }

    return {0 ,0};
}

glm::vec2 DynamicColliderComponent::checkDynamicCollisionDirection(DynamicColliderComponent *dynamicComp, glm::vec2 myPos) const {
    // Distance after which circles will touch
    float circlesLimit = radius + dynamicComp->getRadius();
    glm::vec2 compPos = dynamicComp->getCenter();

    if(powf(myPos.x - compPos.x, 2) + powf(myPos.y - compPos.y, 2) >= circlesLimit * circlesLimit){
        return {0 ,0};
    }

    glm::vec2 norVec = myPos - compPos;
    norVec = glm::normalize(norVec);

    // Normalised distance vector * (float) distance between circles
    return norVec * (circlesLimit - sqrtf(powf(myPos.x - compPos.x, 2) + powf(myPos.y - compPos.y, 2)));
}

void DynamicColliderComponent::setCenterOffset(glm::vec2 newCenterOffset) {
    centerOffset = newCenterOffset;
}

glm::vec2 DynamicColliderComponent::getCenter() const {
    glm::vec3 circlePosition3d = parent->transform->getLocalPosition();
    glm::vec2 circlePosition = {circlePosition3d.x, circlePosition3d.z};
    circlePosition += centerOffset;
    return circlePosition;
}

float DynamicColliderComponent::getRadius() const{
    return radius;
}

void DynamicColliderComponent::setRadius(float newRadius){
    radius = newRadius;
}

glm::vec2 DynamicColliderComponent::getCenterOffset() {
    return centerOffset;
}

const std::vector<StaticColliderComponent *> &DynamicColliderComponent::getTouchingStaticComponents() const {
    return touchingStaticComponents;
}

const std::vector<DynamicColliderComponent *> &DynamicColliderComponent::getTouchingDynamicComponents() const {
    return touchingDynamicComponents;
}

void DynamicColliderComponent::setTileColliderIAmOn() {
    float minDistance = std::numeric_limits<float>::max();
    StaticColliderComponent* closestComponent = nullptr;
    glm::vec2 myPosition = getCenter();

    for(StaticColliderComponent* component : touchingStaticComponents){
        float distance = glm::length(component->getCenter() + myPosition);
        if(distance < minDistance){
            closestComponent = component;
            minDistance = distance;
        }
    }

    tileIAmOn = closestComponent;
}

StaticColliderComponent* DynamicColliderComponent::getTileColliderIAmOn() {
    return tileIAmOn;
}
