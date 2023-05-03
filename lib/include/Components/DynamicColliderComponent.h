#ifndef OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
#define OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H

#include <vector>
#include "Component.h"

class TimeManager;
class World;
class StaticColliderComponent;
class DynamicColliderComponent : public Component {
private:
    World* world;
    TimeManager* timeManager;
    float radius;
    glm::vec2 centerOffset;
    std::vector<StaticColliderComponent*> touchingStaticComponents;
    std::vector<DynamicColliderComponent*> touchingDynamicComponents;

public:
    DynamicColliderComponent(Entity *parent, float radius, glm::vec2 centerOffset = {0, 0});

    void update() override;

    void checkAllCollisions();

    [[nodiscard]] StaticColliderComponent* getTileColliderIAmOn() const;

    [[nodiscard]] const std::vector<StaticColliderComponent *> &getTouchingStaticComponents() const;

    [[nodiscard]] const std::vector<DynamicColliderComponent *> &getTouchingDynamicComponents() const;

    glm::vec2 getCenterOffset();
    void setCenterOffset(glm::vec2 newCenterOffset);

    [[nodiscard]] glm::vec2 getCenter() const;

    [[nodiscard]] float getRadius() const;
    void setRadius(float newRadius);

private:
    glm::vec2 checkStaticCollisionDirection(StaticColliderComponent* statComp, glm::vec2 circlePosition) const;

    glm::vec2 checkDynamicCollisionDirection(DynamicColliderComponent *dynamicComp, glm::vec2 myPos) const;
};

#endif //OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
