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
	pickUps.push_back(createPickUp({ 1,0,0 }));
	pickUps.push_back(createPickUp({ 0,1,0 }));
	pickUps.push_back(createPickUp({ 1,0,1 }));
}

void Spawner::update() {
	if (!isSpawned)
	{
		if (timer < timeToSpawn) {
			timer += timeManager->getDeltaTime120FPS();
		}
		else {
			currentItem = drawPickUp();
			parent->addChild(currentItem);
			currentItem->forceUpdateSelfAndChild();
			world->clearReloadLists();
			timer = 0.0f;
			isSpawned = true;
		}
	}
	else {
		//std::cout << currentItem->components.size() << std::endl;
	}
	
}

Entity* Spawner::createPickUp(glm::vec3 color) {

	Entity* spawndItem = new Entity("res/models/powerUp.obj", shader);
	spawndItem->transform->addToLocalPosition({ 0.0,0.18,0.0 });
	DynamicColliderComponent* col =  new DynamicColliderComponent(spawndItem, 0.1f, true);
	spawndItem->addComponent(col);
	spawndItem->addComponent(new PickUp(spawndItem, this, col, color));
	return spawndItem;
}

Entity* Spawner::drawPickUp(){
	int random = rand() % pickUps.size();
	return pickUps[random];
}

void Spawner::disablePickUp() {
	parent->children.pop_back();
	world->clearReloadLists();
	isSpawned = false;
}