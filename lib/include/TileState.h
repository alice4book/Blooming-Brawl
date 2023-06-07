#pragma once
#include "Enums/PlayerIDType.h"
#include "Enums/TileStateType.h"
#include "Component.h"
#include "glm/vec2.hpp"
#include "Node.h"
#include "TimeManager.h"

class Model;
class Map;

class TileState :
    public Component
{
private:
    EPlayerID ownerID = EPlayerID::None;
    Model* tileModels;
    Map* map;
    TimeManager* timeManager;
    float timerGrow = 0;
    float timerBurn = 0;
    float defaultGrowTime = 8;
    float defaultBurnTime = 6;
    float watered = false;
public:
    EState state = EState::Empty;
    glm::vec2 mapPosition;
    Entity* neighbours[8] = {}; //neighbouring tiles or NULL
    Node node;
    float currentGrowTime = 8;
    float currentBurnTime = 6;
    TileState(Entity* parent, EState state, Model* tileModels, glm::vec2 mapPosition, Map* map);
    void attachToTimeManager();
    void setState(EState newState);
    void changeTileState(EPlayerID playerID);
    EPlayerID getOwner();
    void update() override;
    void setTimerGrow();
    void setTimerBurn();
    void water();
};

