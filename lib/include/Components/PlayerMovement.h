#pragma once

#include "Component.h"
#include "PlayerIDType.h"
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

	int axisCount;
	const float* axes;

    GLFWwindow* window;
    TimeManager* timeManager;

	int axisCount2;
	const float* axes2;

    void move();
    void checkInput();
    void handleSeenTile();

	void reactToPunch(Entity* punchedParent);

	void reactToPunchRobot();

public:
	PlayerMovement(GLFWwindow* window, Entity* parent, Entity* rivalParent, Entity* robot, Transform* transform, DynamicColliderComponent* collider, DynamicColliderComponent* forntCollider, float speed, EPlayerID ID, glm::vec3 forward = { 1, 0, 0 });
    void update() override;
	void setSpeed(float newSpeed);
	float getSpeed();
};