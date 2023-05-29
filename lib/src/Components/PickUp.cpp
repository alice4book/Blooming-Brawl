#include "PickUp.h"
#include "glm/glm.hpp"
#include "DynamicColliderComponent.h"
#include "Spawner.h"
#include "Entity.h"
#include "TimeManager.h"
#include "PlayerIDType.h"
#include "PlayerMovement.h"
#include "World.h"
#include "Shader.h"
#include "Map.h"
#include <iostream>


PickUp::PickUp(Entity* parent, Spawner* spawner, DynamicColliderComponent* collider, glm::vec3 color, EPickUp type = EPickUp::NoPower)
	: Component(parent)
	, colliderBody(collider)
	, spawn(spawner)
	, rimColor(color)
	, type(type)
	, player(nullptr)
{
	timeManager = TimeManager::getInstance();
	timeManager->attach120FPS(this);
	timer = -1.0f;
	parent->setColor(color);
	isSpawn = false;
	isUsed = false;
	pickupMovement = 0.0f;
}

void PickUp::update()
{
	if (pickupMovement > 6.2831f) {
		pickupMovement = pickupMovement - 6.2831f;
	}
	pickupMovement += timeManager->getDeltaTime120FPS() * 2.2f;
	parent->shader->use();
	parent->shader->setFloat("time", pickupMovement);
	if (timer > 0.0f) {
		timer -= timeManager->getDeltaTime120FPS();
		if (timer < 0.0f)
			endUse();
	}
}

void PickUp::use(Entity* player)
{
	if (player != nullptr) {
		isUsed = true;
		this->player = player;
		timeManager->attach120FPS(this);
		timer = 10.0f;
		//auto world = World::getInstance();
		//Map* mapCom = world->mapComponent;
		std::vector<PlayerMovement*> playerCom;
		switch (type) {
			case Speed:
				player->getComponentsByType(&playerCom);
				playerCom[0]->setSpeed(1.6f);
				break;
			case Rain:
				
				break;
			case Meteor:
				break;
		}
		parent->isModel = false;
		colliderBody->enabled = false;
	}
}

void PickUp::endUse()
{
	std::vector<PlayerMovement*> playerCom;
	switch (type) {
	case Speed:
		player->getComponentsByType(&playerCom);
		playerCom[0]->setSpeed(1.0f);
		break;
	case Rain:
		break;
	case Meteor:
		break;
	}
	timeManager->detach(this);
	player = nullptr;
	isUsed = false;
	spawn->disablePickUp();

	parent->isModel = true;
	colliderBody->enabled = true;
}


