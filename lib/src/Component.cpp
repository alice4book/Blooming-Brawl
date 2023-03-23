#include "Component.h"

Component::Component() {
    compType = eNone;
}

ComponentType Component::getComponentType() {
    return compType;
}

bool Component::isComponentType(ComponentType compType) {
    if (this->compType == compType) {
        return true;
    }
    return false;
}