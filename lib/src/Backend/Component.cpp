#include "Component.h"
#include "TimeManager.h"

Entity* Component::getParent()
{
    return parent;
}

Component::~Component() {
    extern TimeManager* timeManager;
    timeManager->detach(this);
}
