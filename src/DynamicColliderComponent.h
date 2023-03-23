#ifndef OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
#define OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H

#include <Component.h>
#include "StaticColliderComponent.h"

class DynamicColliderComponent : Component {
private:
    Entity* parent;
    glm::vec2 center;
    float radius;
    vector<StaticColliderComponent*> staticColliders;

public:
    DynamicColliderComponent(Entity* parent, glm::vec2 center, float radius)
    : parent(parent), center(center), radius(radius){
        compType = "DynamicColliderComponent";

        // Create list of static colliders from whole map
        vector<Entity *> objectsOnScene = parent->getParent()->getChildren();
        for(Entity* singleObj : objectsOnScene){
            Component* component = singleObj->getComponentByType("StaticColliderComponent");
            if(component != nullptr){
                staticColliders.push_back(dynamic_cast<StaticColliderComponent*>(component));
            }
        }
    }

    void CollisionCheck(){
        for(StaticColliderComponent* singleObj : staticColliders){
            // check if two colliders fade over
        }
    }
};

#endif //OPENGLGP_DYNAMICCOLLIDERCOMPONENT_H
