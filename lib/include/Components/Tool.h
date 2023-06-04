#include "Component.h"
#include "glm/glm.hpp"
#include "PlayerIDType.h"

class Entity;
class Transform;
class TimeManager;
class Tool : public Component {
private:
	bool restarting = false;
	bool tool_spawned = false;
	EPlayerID player;
	float timer;
	float timermax;
	TimeManager* manager;

public:
	Tool(Entity* parent);
	bool isRestarting();
	bool isSpawned();
	void resetSpawn();
	void setSpawn();
	void PickedUp(EPlayerID p, Transform*  t = nullptr);
	bool isPickedUp();
	void update() override;
};

