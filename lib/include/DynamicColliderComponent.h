#ifndef OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
#define OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H

#include <vector>
#include "Component.h"

class StaticColliderComponent;
class DynamicColliderComponent : Component {
private:
    float radius;
    std::vector<StaticColliderComponent*> staticColliders;

public:
    DynamicColliderComponent(Entity *parent, float radius);

    void update() override;

    void updateCollidersList();

    void checkAllCollisions();

    [[nodiscard]] glm::vec2 checkCollisionDirection(glm::vec2 squareSize, glm::vec2 squarePosition, glm::vec2 circlePosition);
};

#endif //OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
