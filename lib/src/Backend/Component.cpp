#include "Component.h"
#include "TimeManager.h"

Component::~Component() {
    extern TimeManager* timeManager;
    timeManager->detach(this);
}
