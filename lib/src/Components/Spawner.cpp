#include "imgui_internal.h"
#include "glm/glm.hpp"
#include "Spawner.h"
#include "Entity.h"
#include "World.h"
#include "TimeManager.h"
#include "DynamicColliderComponent.h"
#include <iostream>
#include <PickUp.h>
#include <Transform.h>

Spawner::Spawner(Entity* parent, Shader* shader)
	: Component(parent)
	, shader(shader)
	, timer(0.0f)
	, timeToSpawn(2.0f)
{
	timeManager = TimeManager::getInstance();
	timeManager->attach120FPS(this);
	world = World::getInstance();
	isSpawned = false;
}

void Spawner::update() {
	if (!isSpawned)
	{
		if (timer < timeToSpawn) {
			timer += timeManager->getDeltaTime120FPS();
		}
		else {
			//createPickUp();
			timer = 0.0f;
			isSpawned = true;
		}
	}
	else {
		//std::cout << parent->children.size() << std::endl;
	}
}


void Spawner::createPickUp()
{
	spawndItem = Entity("res/models/powerUp.obj", shader);
	spawndItem.transform->addToLocalPosition({ 0.0,0.18,0.0 });
	DynamicColliderComponent* col =  new DynamicColliderComponent(&spawndItem, 0.1f, true);
	spawndItem.addComponent(col);
	spawndItem.addComponent(new PickUp(&spawndItem, col));
	parent->addChild(&spawndItem);
	spawndItem.forceUpdateSelfAndChild();

	world->clearReloadLists();
};
