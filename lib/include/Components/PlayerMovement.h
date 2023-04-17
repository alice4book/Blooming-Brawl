#pragma once

#include "Component.h"
#include "PlayerIDType.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"

class Entity;
class Transform;
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
	float currentFrame;
	float deltaTime;
	float lastFrame;

	int axisCount;
	const float* axes;

	//int axisCount2;
	//const float* axes2;

public:
	PlayerMovement(Entity* parent, Transform* transform, DynamicColliderComponent* collider, float speed, EPlayerID ID, glm::vec3 forward = { 1, 0, 0 });
	void move(GLFWwindow* window);
};