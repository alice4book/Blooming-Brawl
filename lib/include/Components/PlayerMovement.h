#pragma once

#include "Component.h"
#include "PlayerIDType.h"
#include "ActionType.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"

class Tool;
class Entity;
class Transform;
class TimeManager;
class DynamicColliderComponent;
class StaticColliderComponent;
class PickUp;
class Player;
class RobotMovement;
class TileState;

class PlayerMovement : public Component {
private:
	glm::vec3 forward;
	glm::vec3 previousForward;
	glm::vec3 setForward;
	float side;
	float speed;
	Tool* tool;
	Transform* transform;
	DynamicColliderComponent* collider;
	DynamicColliderComponent* frontCollider;
	StaticColliderComponent* lastSeenTile = nullptr;
	EPlayerID ID;
	Entity* rivalParent;
	Entity* robot;
	PlayerMovement* rival;

	int axisCount;
	const float* axes;

    GLFWwindow* window;
    TimeManager* timeManager;

	int axisCount2;
	const float* axes2;

	//timers
	float actionTimer = 0;
	bool isTimerSet = false;
	TileState* actionTile;
	EActionType currentAction = Idle;

	float currentPlantTime = 1;
	float currentDestroyTime = 1;
	float currentHitTime = 1;
	float currentWaterTime = 1;
	float currentHarvestTime = 1;

	float defaultPlantTime = 1;
	float defaultDestroyTime = 1;
	float defaultHitTime = 1;
	float defaultWaterTime = 1;
	float defaultHarvestTime = 1;

    void move();
    void checkInput();

	void reactToPunch(Entity* punchedParent);

	void reactToPunchRobot();
    void handleSeenTile();
	void handleActionTimer();

public:
	PlayerMovement(GLFWwindow* window, Entity* parent, Entity* rivalParent, Entity* robot, Transform* transform, DynamicColliderComponent* collider, DynamicColliderComponent* forntCollider, float speed, EPlayerID ID, glm::vec3 forward = { 1, 0, 0 });
    void update() override;
	void setSpeed(float newSpeed);
	float getSpeed();
	void dropTool();
	void resetSeenTile();
	void startAction(TileState* tile);
	void cancelAction(TileState* tile);
	void setRivalPlayerMovement(PlayerMovement* rivalPlayerMovement);
};