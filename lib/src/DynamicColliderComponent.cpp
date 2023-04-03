#include "World.h"
#include "Transform.h"
#include "StaticColliderComponent.h"
#include "DynamicColliderComponent.h"

DynamicColliderComponent::DynamicColliderComponent(Entity *parent, float radius, glm::vec2 centerOffset)
: Component(parent), radius(radius), centerOffset(centerOffset), bColliderFlag(false) {
    compType = eCollisionDynamic;
    world = World::getInstance();
}

void DynamicColliderComponent::update() {
    Component::update();
    checkAllCollisions();
}

void DynamicColliderComponent::checkAllCollisions(){
    glm::vec2 circlePosition = getCenter();
    touchingComponents.clear();
    bColliderFlag = false;

    // Tools, map tiles
    for(StaticColliderComponent* statComp : world->getStaticColliders()){
         //check if two colliders fade over
          glm::vec2 collisionDirection = checkStaticCollisionDirection(statComp, circlePosition);
        if(collisionDirection.x + collisionDirection.y != 0) {
            if (!statComp->getIsPassable()) {
                bColliderFlag = true;
                parent->transform->addToLocalPosition({collisionDirection.x, 0, collisionDirection.y});
            } else {
                touchingComponents.push_back((Component *) statComp);
            }
        }
    }
    // Robot, enemy player
    for(DynamicColliderComponent* dynamicComp : world->getDynamicColliders()){
        if(this != dynamicComp){
            glm::vec2 colDir = checkDynamicCollisionDirection(dynamicComp, circlePosition);
            parent->transform->addToLocalPosition({colDir.x, 0, colDir.y});
            touchingComponents.push_back((Component*)dynamicComp);
        }
    }
}

glm::vec2 DynamicColliderComponent::checkStaticCollisionDirection(StaticColliderComponent* statComp, glm::vec2 circlePosition) {
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

    return {0, 0};
}

glm::vec2 DynamicColliderComponent::checkDynamicCollisionDirection(DynamicColliderComponent *dynamicComp, glm::vec2 myPos) {
    // Distance after which circles will touch
    float circlesLimit = radius + dynamicComp->getRadius();
    glm::vec2 compPos = dynamicComp->getCenter();

    if(powf(myPos.x - compPos.x, 2) + powf(myPos.y - compPos.y, 2) >= circlesLimit * circlesLimit){
        return {0 ,0};
    }

    glm::vec2 norVec = myPos - compPos;
    glm::normalize(norVec);

    // Normalised distance vector * (float) distance between circles
    return norVec * (circlesLimit - sqrtf(powf(myPos.x - compPos.x, 2) + powf(myPos.y - compPos.y, 2)));
}

bool DynamicColliderComponent::getColliderFlag() {
    return bColliderFlag;
}

void DynamicColliderComponent::setCenterOffset(glm::vec2 newCenterOffset) {
    centerOffset = newCenterOffset;
}

glm::vec2 DynamicColliderComponent::getCenter(){
    glm::vec3 circlePosition3d = parent->transform->getLocalPosition();
    glm::vec2 circlePosition = {circlePosition3d.x, circlePosition3d.z};
    circlePosition += centerOffset;
    return circlePosition;
}

float DynamicColliderComponent::getRadius(){
    return radius;
}

void DynamicColliderComponent::setRadius(float newRadius){
    radius = newRadius;
}

glm::vec2 DynamicColliderComponent::getCenterOffset() {
    return centerOffset;
}




