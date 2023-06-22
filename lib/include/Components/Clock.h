#pragma once
#include "Component.h"

class Entity;
class TimeManager;

class Clock : public Component {

private:

	TimeManager* timeManager;

	int seconds;

public:
	Clock(Entity* parent);

	void update() override;

	void startClock(int seconds);

	void endBreakClock();

	void endLevelClock();

	void detachClock();

	int getSeconds();
};