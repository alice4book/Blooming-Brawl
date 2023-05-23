#include "PickUp.h"
#include "glm/glm.hpp"
#include "DynamicColliderComponent.h"
#include "Spawner.h"
#include "Entity.h"
#include "TimeManager.h"
#include "PlayerIDType.h"
#include "PlayerMovement.h"
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
}

void PickUp::update()
{
	if (timer > 0.0f) {
		timer -= timeManager->getDeltaTime120FPS();
		if (timer < 0.0f)
			endUse();
	}
}

void PickUp::use(Entity* player)
{
	if (player != nullptr) {
		this->player = player;
		timeManager->attach120FPS(this);
		switch (type) {
			case Speed:
				timer = 10.0f;
				std::vector<PlayerMovement*> playerCom;
				player->getComponentsByType(&playerCom);
				playerCom[0]->setSpeed(1.6f);
				break;
		}
		parent->isModel = false;
		parent->model = nullptr;
		colliderBody = nullptr;
	}
}

void PickUp::endUse()
{
	if (type == Speed) {
		std::vector<PlayerMovement*> playerCom;
		this->player->getComponentsByType(&playerCom);
		playerCom[0]->setSpeed(1.0f);
	}
	timeManager->detach(this);
	player = nullptr;
	spawn->disablePickUp();
}


