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
#include "Enums/PickupType.h"

Spawner::Spawner(Entity* parent, Shader* shader)
	: Component(parent)
	, shader(shader)
	, timer(0.0f)
	, timeToSpawn(4.0f)
{
	timeManager = TimeManager::getInstance();
	timeManager->attach120FPS(this);
	world = World::getInstance();
	isSpawned = false;
	//pickUps.push_back(createPickUp({ 0.2,0.2,1 }, Rain));
	pickUps.push_back(createPickUp({ 0,1,0 }, Locust));
	pickUps.push_back(createPickUp({ 1,0,1 }, Speed));
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
}

Entity* Spawner::createPickUp(glm::vec3 color, EPickUp type) {
	Entity* spawndItem = new Entity("res/models/powerUp.obj", shader);
	spawndItem->transform->addToLocalPosition({ 0.0,0.1,0.0 });
	DynamicColliderComponent* col =  new DynamicColliderComponent(spawndItem, 0.05f, true);
	spawndItem->addComponent(col);
	spawndItem->addComponent(new PickUp(spawndItem, this, col, color, type));
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
	timeToSpawn = 4.0f;
}