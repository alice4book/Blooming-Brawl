#ifndef OPENGLGP_STATICCOLLIDERCOMPONENT_H
#define OPENGLGP_STATICCOLLIDERCOMPONENT_H

#include "Component.h"
#include "glm/vec2.hpp"

class StaticColliderComponent : Component {
private:
    glm::vec2 center;
    glm::vec2 size;
    bool isPassable;
public:
    StaticColliderComponent(Entity *parent, glm::vec2 center, glm::vec2 size, bool isPassable);
};

#endif //OPENGLGP_STATICCOLLIDERCOMPONENT_H
