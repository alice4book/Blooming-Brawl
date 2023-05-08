#include "TileState.h"
#include "StaticColliderComponent.h"
#include "Entity.h"
#include "Shader.h"

TileState::TileState(Entity* parent, EState state, Model* tileModels, glm::vec2 mapPosition)
 : Component(parent), state(state), tileModels(tileModels), mapPosition(mapPosition)
{}

void TileState::changeTileState(EPlayerID playerID) {
    // TODO: Paulina will do this
}
