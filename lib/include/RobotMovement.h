#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

class Transform;
class DynamicColliderComponent;

class RobotMovement : Component{
private:
	glm::vec3 forward;
	float side;
	float speed;
	Transform* transform;
	DynamicColliderComponent* collider;
	float currentFrame;
	float deltaTime;
	float lastFrame;
	
public:
	RobotMovement(Entity* parent, Transform* transform, DynamicColliderComponent* collider, float speed, glm::vec3 forward = {1, 0, 0});
	void update() override;
	void turnRight(float dTime);
};