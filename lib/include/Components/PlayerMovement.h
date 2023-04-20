#pragma once

#include "Component.h"
#include "PlayerIDType.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"

class Entity;
class Transform;
class TimeManager;
class DynamicColliderComponent;

class PlayerMovement : public Component {
private:
	glm::vec3 forward;
	glm::vec3 previousForward;
	glm::vec3 setForward;
	float side;
	float speed;
	Transform* transform;
	DynamicColliderComponent* collider;
	EPlayerID ID;

	int axisCount;
	const float* axes;

    GLFWwindow* window;
    TimeManager* timeManager;

	//int axisCount2;
	//const float* axes2;

    void move();

public:
	PlayerMovement(GLFWwindow* window, Entity* parent, Transform* transform, DynamicColliderComponent* collider, float speed, EPlayerID ID, glm::vec3 forward = { 1, 0, 0 });
    void update() override;
};