#ifndef OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
#define OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H

#include <vector>
#include "glm/vec2.hpp"
#include "Component.h"

class StaticColliderComponent;
class DynamicColliderComponent : Component {
private:
    glm::vec2 center;
    float radius;
    std::vector<StaticColliderComponent*> staticColliders;

public:
    DynamicColliderComponent(Entity *parent, glm::vec2 center, float radius);

    void CollisionCheck();
};

#endif //OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
