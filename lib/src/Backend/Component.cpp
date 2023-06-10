#include "Component.h"
#include "TimeManager.h"
#include <iostream>

Entity* Component::getParent()
{
    return parent;
}

Component::~Component() {
    extern TimeManager* timeManager;
    timeManager->detach(this);

    //std::cout << " Component delete " << std::endl;
}
