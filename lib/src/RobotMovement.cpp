#include "RobotMovement.h"

#include <iostream>   // std::cout
#include <string> 
#include <imgui_internal.h>

#include "DynamicColliderComponent.h"
#include "Transform.h"

RobotMovement::RobotMovement(Entity* parent, Transform* transform, 
	DynamicColliderComponent* collider, float speed, RobotMovementType type, 
	glm::vec3 forward)
	: Component(parent)
	,forward(forward)
	,speed(speed)
	,transform(transform)
	,collider(collider)
	,deltaTime(0.f)
{
	if(forward.x == 1.0f && forward.z == 0.0f){
		side = 0.0;
	}else
	if(forward.x == -1.0f && forward.z == 0.0f){
		side = 180.0;
	}else
	if(forward.x == 0.0f && forward.z == 1.0f){
		side = 90.0;
	}else
	if(forward.x == 0.0f && forward.z == -1.0f){
		side = 270.0;
	}
	else {
		side = 0.0;
	}
	switch (type) {
	case eLeft:
		moveRob = &RobotMovement::turnLeft;
		break;
	case eRight:
		moveRob = &RobotMovement::turnRight;
		break;
	case eStop:
		moveRob = &RobotMovement::noMove;
		break;
	default:
		moveRob = &RobotMovement::noMove;
	}
	currentFrame = static_cast<float>(glfwGetTime());
	lastFrame = currentFrame;
}

void RobotMovement::update() {	
	currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	transform->addToLocalPosition(forward * (speed * deltaTime));
	if(collider->getColliderFlag())
	{
		(this->*moveRob)(deltaTime);
	}
}

// turns robot right (only with right angle)
void RobotMovement::turnRight(float dTime) {
	side += 90.f;
	if(side > 270.f)
		side = 0.0f;
	transform->addToLocalPosition(forward * -(speed * dTime));
	transform->rotateLocal(glm::vec3(0.0f, -90.0f, 0.0f));
	glm::vec3 front;
	front.x = cos(glm::radians(side)) * cos(glm::radians(0.f));
	front.y = sin(glm::radians(0.f));
	front.z = sin(glm::radians(side)) * cos(glm::radians(0.f));
	forward = glm::normalize(front);
}

// turns robot left (only with right angle)
void RobotMovement::turnLeft(float dTime) {
	side -= 90.f;
	if (side < 0.f)
		side = 270.0f;
	transform->addToLocalPosition(forward * -(speed * dTime));
	transform->rotateLocal(glm::vec3(0.0f, 90.0f, 0.0f));
	glm::vec3 front;
	front.x = cos(glm::radians(side)) * cos(glm::radians(0.f));
	front.y = sin(glm::radians(0.f));
	front.z = sin(glm::radians(side)) * cos(glm::radians(0.f));
	forward = glm::normalize(front);
}

void RobotMovement::noMove(float dTime) {
	transform->addToLocalPosition(forward * -(speed * dTime));
}
