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

	//punching
	bool rivalPunched = false;
	float rivalPunchTimer = 0.0f;
	float rivalPunchTime = 0.2f;
	glm::vec3 rivalPunchedDistance;
	bool robotPunched = false;
	float robotPunchTimer = 0.0f;
	float robotPunchTime = 0.2f;
	glm::vec3 robotPunchedDistance = glm::vec3(0.1,0,0.1);

	//timers
	float actionTimer = 0;
	bool isTimerSet = false;
	TileState* actionTile;
	EActionType currentAction = Idle;

	float currentPlantTime = 0.4;
	float currentDestroyTime = 0.4;
	float currentHitTime = 0.4;
	float currentWaterTime = 0.4;
	float currentHarvestTime = 0.4;

	float defaultPlantTime = 0.4;
	float defaultDestroyTime = 0.4;
	float defaultHitTime = 0.4;
	float defaultWaterTime = 0.4;
	float defaultHarvestTime = 0.4;

    void move();
    void checkInput();

	void reactToPunch(Entity* punchedParent);

	void updateRivalAfterPunch(glm::vec3 distance);

	void updateRobotAfterPunch(glm::vec3 distance);

	void reactToPunchRobot();
    void handleSeenTile();
	void handleActionTimer();

public:
	PlayerMovement(GLFWwindow* window, Entity* parent, Entity* rivalParent, Entity* robot, Transform* transform, DynamicColliderComponent* collider, DynamicColliderComponent* forntCollider, float speed, EPlayerID ID, glm::vec3 forward = { 1, 0, 0 });
    void update() override;
	void enable(bool value) override;
	void setSpeed(float newSpeed);
	float getSpeed();
	void dropTool();
	void resetSeenTile();
	void startAction(TileState* tile);
	void cancelAction(TileState* tile = nullptr);
	void setRivalPlayerMovement(PlayerMovement* rivalPlayerMovement);
};