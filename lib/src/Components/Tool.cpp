#include "Tool.h"
#include "Entity.h"
#include "Transform.h"
#include "TimeManager.h"

bool Tool::isSpawned() {
	return tool_spawned;
}
void Tool::resetSpawn() {
	tool_spawned = false;
}
void Tool::setSpawn() {
	tool_spawned = true;
}
void Tool::PickedUp(EPlayerID p, Transform* t) {
	if (p == None) {
		restarting = true;
		parent->isModel = true;
		parent->transform->setLocalPosition(t->getLocalPosition());
		player = p;
		timermax = 1;
	}
	else {
		parent->isModel = false;
		player = p;
	}
}

void Tool::update() {
	if (timer < timermax) {
		timer += manager->getDeltaTime120FPS();
		if (timer > timermax) {
			restarting = false;
			timer = 0;
			timermax = 0;
		}
	}
}

bool Tool::isRestarting() {
	return restarting;
}

bool Tool::isPickedUp() {
	if (player == None) {
		return false;
	}
	return true;
}
Tool::Tool(Entity* parent) : Component(parent) {
	player = None;
	timermax = 0;
	timer = 0;
	manager = TimeManager::getInstance();
	manager->attach120FPS(this);
};