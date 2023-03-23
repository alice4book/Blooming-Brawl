#ifndef OPENGLGP_STATICCOLLIDERCOMPONENT_H
#define OPENGLGP_STATICCOLLIDERCOMPONENT_H

#include <Component.h>
#include "Entity.h"

class StaticColliderComponent : Component {
private:
    Entity* parent;
    glm::vec2 center;
    glm::vec2 size;
    bool isImpassable;
public:
    StaticColliderComponent(Entity* parent, glm::vec2 center, glm::vec2 size, bool isImpassable)
    : parent(parent), center(center), size(size), isImpassable(isImpassable){
        compType = "StaticColliderComponent";
    }
};

#endif //OPENGLGP_STATICCOLLIDERCOMPONENT_H
