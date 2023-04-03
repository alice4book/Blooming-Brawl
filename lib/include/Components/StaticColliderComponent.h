#include "Component.h"
#include "glm/vec2.hpp"

class StaticColliderComponent : public Component {
private:
    glm::vec2 size;
    bool isPassable;
public:
    StaticColliderComponent(Entity *parent, glm::vec2 size, bool isPassable);

    [[nodiscard]] const glm::vec2 &getSize() const;

    [[nodiscard]] bool getIsPassable() const;

    glm::vec2 getCenter();
};

