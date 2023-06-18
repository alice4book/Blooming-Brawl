#include "TileState.h"
#include "StaticColliderComponent.h"
#include "Entity.h"
#include "Shader.h"
#include "Model.h"
#include "Map.h"
#include "ActionType.h"

TileState::TileState(Entity* parent, EState state, Model* tileModels, glm::vec2 mapPosition, Map* map)
 : Component(parent), state(state), tileModels(tileModels), mapPosition(mapPosition), map(map)
{
    timeManager = TimeManager::getInstance();
}

void TileState::attachToTimeManager()
{
    timeManager->attachUnlimitedFPS(this);
}

void TileState::setState(EState newState)
{
	state = newState;
	parent->model = tileModels + state;
    if (state == EState::Growing || state == EState::Growing2) {
        setTimerGrow();
    }
    else if (state == EState::Burned) {
        setTimerBurn();
    }
}

void TileState::changeTileState(EPlayerID playerID, EActionType actionType)
{
    if (playerID == EPlayerID::RobotDestroyer)
    {
        if (state == Growing || state == Growing2 || state == Grown || state == Grown2)
        setState(EState::Burned);
        if (ownerID == EPlayerID::Player1) {
            map->addToPlayer1TilesCount(-1);
        }
        else if (ownerID == EPlayerID::Player2) {
            map->addToPlayer2TilesCount(-1);
        }
        ownerID = EPlayerID::None;
    }
    else
    {
        switch (actionType)
        {
        case Planting:
            ownerID = playerID;
            if(map->getSeedCount(playerID) > 0){
                map->subSeedsForPlanting(playerID);
                    if (playerID == EPlayerID::Player1) {
                        setState(EState::Growing);
                        map->addToPlayer1TilesCount(1);
                    }
                    else {
                        setState(EState::Growing2);
                        map->addToPlayer2TilesCount(1);
                    }
                }
            break;
        case DestroyingFlower:
            setState(EState::Burned);
            if (ownerID == EPlayerID::Player1) {
                map->addToPlayer1TilesCount(-1);
            }
            else if (ownerID == EPlayerID::Player2) {
                map->addToPlayer2TilesCount(-1);
            }
            ownerID = EPlayerID::None;
            break;
        case DestroyingOvergrown:
            setState(EState::Empty);
            map->addSeedsFromPlants(playerID);
            break;
        case Harvesting:
            map->addSeedsFromPlants(playerID);
            if (playerID == EPlayerID::Player1)
            {
                setState(EState::Growing);
            }
            else
            {
                setState(EState::Growing2);
            }
            break;
        case Watering:
            if (!watered)
            {
                watered = true;
                timerGrow -= currentGrowTime * 0.5;
                //TODO: show somehow that tile is watered
            }
            break;
        }
    }
}

EPlayerID TileState::getOwner()
{
    return ownerID;
}

void TileState::update()
{
    if (state == EState::Burned)
    {
        timerBurn -= timeManager->getDeltaTimeUnlimitedFPS();
        if (timerBurn <= 0)
        {
            setState(EState::Empty);
            timerBurn = 0;
            watered = false;
        }
    }
    else if (state == EState::Growing || state == EState::Growing2)
    {
        timerGrow -= timeManager->getDeltaTimeUnlimitedFPS() * growingSpeed;
        if (timerGrow <= 0)
        {
            if (ownerID == EPlayerID::Player1)
                setState(EState::Grown);
            else setState(EState::Grown2);
            timerGrow = 0;
            watered = false;
        }
    }
}

void TileState::resetGrowingSpeed() {
    growingSpeed = 1.0f;
}

void TileState::setTimerGrow()
{
    timerGrow = currentGrowTime;
}

void TileState::setTimerBurn()
{
    timerBurn = currentBurnTime;
}

void TileState::water()
{
    if (!watered)
    {
        watered = true;
        timerGrow -= currentGrowTime * 0.5;
    }
}

