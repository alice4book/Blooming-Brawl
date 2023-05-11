#include "Enums/PlayerIDType.h"
#include "Enums/TileStateType.h"
#include "Component.h"
#include "glm/vec2.hpp"
#include "Node.h"

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
    glm::vec2 mapPosition;
    Entity* neighbours[8] = {}; //neighbouring tiles or NULL
    Node node;

    TileState(Entity* parent, EState state, Model* tileModels, glm::vec2 mapPosition);

    void setState(EState newState);
    void changeTileState(EPlayerID playerID);
};

