#include "Clock.h"
#include "TimeManager.h"
#include "HUD.h"

Clock::Clock(Entity* parent) : Component(parent)
{
	timeManager = TimeManager::getInstance();
	seconds = -1;
}

void Clock::update()
{
	if (seconds <= 0) {
		stopClock();
		return;
	}
	seconds--;
}

void Clock::startClock(int seconds)
{
	this->seconds = seconds;
	timeManager->attach1FPS(this);
}

void Clock::stopClock()
{
	timeManager->detach(this);
	HUD* hud = dynamic_cast<HUD*>(parent);
	if (hud != nullptr)
		hud->nextMap();

}

int Clock::getSeconds()
{
	return seconds;
}

