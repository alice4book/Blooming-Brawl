#include "imgui_internal.h"
#include "Spawner.h"
#include "Entity.h"
#include "TimeManager.h"
#include <iostream>

Spawner::Spawner(Entity* parent, Shader* shader)
	: Component(parent)
	, shader(shader)
	, timer(0.0f)
	, timeToSpawn(5.0f)
	, spawndItem(nullptr)
{
	timeManager = TimeManager::getInstance();
	timeManager->attach120FPS(this);
	
}

void Spawner::update() {
	if (spawndItem == nullptr) 
	{
		if (timer < timeToSpawn) {
			timer += timeManager->getDeltaTime120FPS();
		}
		else {
			createPickUp();
			timer = 0.0f;
		}
	}
}

Entity* Spawner::spawnPickUp()
{
	//spawndItem = new Entity();
	return nullptr;
};

Entity* Spawner::createPickUp()
{
	spawndItem = new Entity("res/models/powerUp.obj", shader);
	parent->addChild(spawndItem);
	return spawndItem;
};
