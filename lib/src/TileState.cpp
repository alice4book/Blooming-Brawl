#include "TileState.h"
#include "StaticColliderComponent.h"
#include "Entity.h"
#include "Shader.h"
#include "Model.h"
#include "Map.h"
#include <iostream>

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
    if (state == EState::Growing || state == EState::Growing2)
        setTimerGrow();
    else if (state == EState::Burned)
        setTimerBurn();
}

void TileState::changeTileState(EPlayerID playerID) {

    if (ownerID == EPlayerID::None)
    {
        if (playerID == Player1 || playerID == Player2)
        {
            if (state == EState::Overgrown)
            {
                setState(EState::Empty);
                //SlingSeed(playerID);
                //SlingSeed(playerID);
            }
            else if (state == EState::Empty)
            {
               ownerID = playerID;
               if (playerID == EPlayerID::Player1){
                    setState(EState::Growing);
                    map->addToPlayer1TilesCount(1);
               }
               else {
                    setState(EState::Growing2);
                    map->addToPlayer2TilesCount(1);
               }
            }
        }
    }
    else if (playerID == ownerID)   // Tile has owner. Owner of tile called.
    {
        if (playerID == EPlayerID::Player1 && state == EState::Grown)
        {
            //SlingSeed(playerID);
            //SlingSeed(playerID);
            setState(EState::Growing);
        }
        if (playerID == EPlayerID::Player2 && state == EState::Grown2)
        {
            //SlingSeed(playerID);
            //SlingSeed(playerID);
            setState(EState::Growing2);
        }
    }
    else if (playerID == EPlayerID::Player1 || playerID == EPlayerID::Player2 || playerID == EPlayerID::RobotDestroyer)  // Tile has owner. Enemy player called.
    {
        setState(EState::Burned);
        if (ownerID == EPlayerID::Player1) {
            map->addToPlayer1TilesCount(-1);
        }
        else if (ownerID == EPlayerID::Player2) {
            map->addToPlayer2TilesCount(-1);
        }
        ownerID = EPlayerID::None;
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
        timerGrow -= timeManager->getDeltaTimeUnlimitedFPS();
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
