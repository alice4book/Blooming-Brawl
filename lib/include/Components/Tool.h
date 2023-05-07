#include "Component.h"
#include "glm/glm.hpp"

class Entity;
class Tool : public Component {
private:
	bool tool_spawned = false;
public:
	Tool(Entity* parent);
	bool isSpawned();
	void resetSpawn();
	void setSpawn();
};

