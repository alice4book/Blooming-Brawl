#include "TileState.h"
#include "StaticColliderComponent.h"
#include "Entity.h"
#include "Shader.h"
#include "Model.h"

TileState::TileState(Entity* parent, EState state, Model* tileModels, glm::vec2 mapPosition)
 : Component(parent), state(state), tileModels(tileModels), mapPosition(mapPosition)
{}

void TileState::setState(EState newState)
{
	state = newState;
	parent->model = tileModels + state;
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
               if (playerID == EPlayerID::Player1)
                    setState(EState::Growing);
                else
                    setState(EState::Growing2);
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
    }
    else if (playerID == EPlayerID::RobotDestroyer) // Tile has owner. Robot called.
    {
        setState(EState::Burned);
    }
}
