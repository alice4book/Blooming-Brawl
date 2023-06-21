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
		HUD* hud = dynamic_cast<HUD*>(parent);
		if (hud->getShowBanner()) {
			endBreakClock();
			return;
		}
		endLevelClock();
		return;
	}
	seconds--;
}

void Clock::startClock(int seconds)
{
	this->seconds = seconds;
	timeManager->attach1FPS(this);
}


void Clock::endBreakClock()
{
	timeManager->detach(this);
	HUD* hud = dynamic_cast<HUD*>(parent);
	if (hud != nullptr) {
		hud->setShowBanner(false);
		hud->nextMap();
	}
}

void Clock::endLevelClock()
{
	timeManager->detach(this);
	HUD* hud = dynamic_cast<HUD*>(parent);
	if (hud != nullptr) {
		hud->setShowBanner(true);
		hud->decideWinner();
		startClock(3);
	}
}

void Clock::detachClock()
{
	timeManager->detach(this);
}

int Clock::getSeconds()
{
	return seconds;
}

