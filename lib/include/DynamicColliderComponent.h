#ifndef OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
#define OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H

#include <vector>
#include "Component.h"

class StaticColliderComponent;
class DynamicColliderComponent : Component {
private:
    float radius;
    glm::vec2 centerOffset;
    std::vector<StaticColliderComponent*> staticColliders;
    std::vector<DynamicColliderComponent*> dynamicColliders;
    bool bColliderFlag;

public:
    std::vector<Component*> touchingComponents;

    DynamicColliderComponent(Entity *parent, float radius, glm::vec2 centerOffset = {0, 0});

    void update() override;

    void updateCollidersList();

    void checkAllCollisions();

    glm::vec2 checkStaticCollisionDirection(StaticColliderComponent* statComp, glm::vec2 circlePosition);

    glm::vec2 checkDynamicCollisionDirection(DynamicColliderComponent *dynamicComp, glm::vec2 myPos);

    bool getColliderFlag();

    void setCenterOffset(glm::vec2 newCenterOffset);

    glm::vec2 getCenterOffset();

    glm::vec2 getCenter();

    float getRadius();

    void setRadius(float newRadius);
};

#endif //OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
