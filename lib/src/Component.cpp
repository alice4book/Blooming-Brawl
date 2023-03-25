#include "Component.h"

Component::Component(Entity* parent) : parent(parent) {

}

ComponentType Component::getComponentType() const {
    return compType;
}

bool Component::isComponentType(ComponentType componentType) {
    if (compType == componentType) {
        return true;
    }
    return false;
}

void Component::update()
{
    
}
