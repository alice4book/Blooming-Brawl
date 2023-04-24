#pragma once
#include "TileState.h"
//#include "Component.h"
//#include "Enums.h"

class Map;

TileState::TileState(Entity* parent, EState state, Model* tileModels, glm::vec2 mapPosition) : Component(parent), state(state), tileModels(tileModels), mapPosition(mapPosition)
{

}
