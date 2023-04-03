#ifndef OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
#define OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H

#include <vector>
#include "Component.h"

class World;
class StaticColliderComponent;
class DynamicColliderComponent : public Component {
private:
    World* world;
    float radius;
    glm::vec2 centerOffset;
    bool bColliderFlag;

public:
    std::vector<Component*> touchingComponents;

    DynamicColliderComponent(Entity *parent, float radius, glm::vec2 centerOffset = {0, 0});

    void update() override;

    void checkAllCollisions();

    glm::vec2 checkStaticCollisionDirection(StaticColliderComponent* statComp, glm::vec2 circlePosition) const;

    glm::vec2 checkDynamicCollisionDirection(DynamicColliderComponent *dynamicComp, glm::vec2 myPos) const;

    [[nodiscard]] bool getColliderFlag() const;

    void setCenterOffset(glm::vec2 newCenterOffset);

    glm::vec2 getCenterOffset();

    glm::vec2 getCenter();

    [[nodiscard]] float getRadius() const;

    void setRadius(float newRadius);
};

#endif //OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
