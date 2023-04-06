#pragma once
#include "Enums/PlayerIDType.h"
#include "Enums/TileStateType.h"
#include "Component.h"

class Model;

class TileState :
    public Component
{
private:
    EPlayerID ownerID = EPlayerID::None;
    Model* tileModels;
    /*float defaultTimerGrow;
    float defaultTimerBurn;
    float timerGrow = 0;
    float timerBurn = 0;
    float watered = false;
    */
public:
    EState state = EState::Empty;

    TileState(Entity* parent, EState state, Model* tileModels);

};

