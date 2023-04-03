#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "RobotMovementType.h"

class Transform;
class DynamicColliderComponent;

class RobotMovement : public Component{
private:
	glm::vec3 forward;
	float side;
	float speed;
	Transform* transform;
	DynamicColliderComponent* colliderBody;
	DynamicColliderComponent* colliderFront;
	float currentFrame;
	float deltaTime;
	float lastFrame;
	float offset;
	//Tells what kind of movement
	RobotMovementType moveType = eStop;
	void (RobotMovement::*moveRob)(float);
	
public:
	RobotMovement(Entity* parent, Transform* transform, DynamicColliderComponent* colliderBody,
		DynamicColliderComponent* colliderFront, float speed, 
		RobotMovementType type, glm::vec3 forward = {1, 0, 0}, float offset = 0.04f);
	void turnRight(float dTime);
	void turnLeft(float dTime);	
	void noMove(float dTime);
	void update() override;
};