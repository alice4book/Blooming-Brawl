#include "Component.h"
#include "glm/vec2.hpp"

class TileState;
class StaticColliderComponent : public Component {
private:
    glm::vec2 size;
    bool isPassable;
    TileState* tileState;
public:
    StaticColliderComponent(Entity *parent, glm::vec2 size, bool isPassable, TileState* tileState = nullptr);

    [[nodiscard]] const glm::vec2 &getSize() const;

    [[nodiscard]] bool getIsPassable() const;

    glm::vec2 getCenter();

    TileState* getTileState();
};

