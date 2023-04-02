#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "RobotMovementType.h"

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
	//Tells what kind of movement
	RobotMovementType moveType = eStop;
	void (RobotMovement::*moveRob)(float);
	
public:
	RobotMovement(Entity* parent, Transform* transform, DynamicColliderComponent* collider, 
		float speed, RobotMovementType type, glm::vec3 forward = {1, 0, 0});
	void turnRight(float dTime);
	void turnLeft(float dTime);	
	void noMove(float dTime);
	void update() override;
};