#pragma once

#include "Component.h"
#include "Enums.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

class Entity;
class Transform;
class DynamicColliderComponent;

class PlayerMovement : public Component {
private:
	glm::vec3 forward;
	float side;
	float speed;
	Transform* transform;
	DynamicColliderComponent* collider;
	EPlayerID ID;
	float currentFrame;
	float deltaTime;
	float lastFrame;

public:
	PlayerMovement(Entity* parent, Transform* transform, DynamicColliderComponent* collider, float speed, EPlayerID ID, glm::vec3 forward = { 1, 0, 0 });
	void move(GLFWwindow* window);
};