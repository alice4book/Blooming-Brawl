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
#include "Player.h"
#include "TileState.h"


PickUp::PickUp(Entity* parent, Spawner* spawner, DynamicColliderComponent* collider, glm::vec3 color, EPickUp type)
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

void PickUp::enable(bool value) {
	if (enabled) {
		TimeManager::getInstance()->detach(this);
		enabled = false;
	}
	if (value) {
		TimeManager::getInstance()->attach120FPS(this);
		enabled = true;
	}
}

void PickUp::use(Entity* player)
{
	if (player != nullptr) {
		isUsed = true;
		this->player = player;
		timeManager->attach120FPS(this);
		timer = 10.0f;
		auto world = World::getInstance();
		Map* mapCom = world->mapComponent;
		std::vector<PlayerMovement*> playerMoveCom;
		std::vector<Player*> playerCom;
		std::vector<TileState*> tileStateCom;
		switch (type) {
			case Speed:
				player->getComponentsByType(&playerMoveCom);
				playerMoveCom[0]->setSpeed(1.6f);
				break;
			case Rain:
				player->getComponentsByType(&playerCom);
				tileStateCom = mapCom->getPlayerTiles(playerCom[0]->getID());
				for (auto tile : tileStateCom) {
					tile->growingSpeed = 1.5f;
				}
				break;
			case Locust:
				int randomTilesCount = 0;
				player->getComponentsByType(&playerCom);
				if (playerCom[0]->getID() == Player1) {
					tileStateCom = mapCom->getPlayerTiles(Player2);
					randomTilesCount = tileStateCom.size() * 30 / 100;
				}
				else if (playerCom[0]->getID() == Player2) {
					tileStateCom = mapCom->getPlayerTiles(Player1);
					randomTilesCount = tileStateCom.size() * 30 / 100;
				}
				int randomTilesNr, randomIter;
				
				std::vector<int> randomNrToUse;
				for (int i = 0; i < tileStateCom.size(); i++) {
					randomNrToUse.push_back(i);
				}
				for (int i = 0; i < randomTilesCount; i++) {
					randomIter = rand() % randomNrToUse.size();
					randomTilesNr = randomNrToUse[randomIter];
					randomNrToUse.erase(randomNrToUse.begin() + randomIter);
					tileStateCom[randomTilesNr]->changeTileState(playerCom[0]->getID(), EActionType::DestroyingFlower);
				}
				
				break;
		}
		parent->isModel = false;
		colliderBody->enable(false);
	}
}

void PickUp::endUse()
{
	auto world = World::getInstance();
	Map* mapCom = world->mapComponent;
	std::vector<Player*> playerCom;
	std::vector<TileState*> tileStateCom;
	switch (type) {
	case Speed:
		player->getComponentsByType(&playerCom);
		playerCom[0]->setSpeed(1.0f);
		break;
	case Rain:
		player->getComponentsByType(&playerCom);
		tileStateCom = mapCom->getPlayerTiles(playerCom[0]->getID());
		for (auto tile : tileStateCom) {
			tile->resetGrowingSpeed();
		}
		break;
	}
	timeManager->detach(this);
	player = nullptr;
	isUsed = false;
	spawn->disablePickUp();

	parent->isModel = true;
	colliderBody->enable(true);
}


