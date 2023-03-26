#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform;
class DynamicColliderComponent;

class RobotMovement : Component{
private:
	glm::vec3 forward;
	float side;
	float speed;
	Transform* transform;
	DynamicColliderComponent* collider;
	
public:
	RobotMovement(Entity* parent, Transform* transform, DynamicColliderComponent* collider, float speed, glm::vec3 forward = {1, 0, 0});
	void update() override;
	void turnRight();

};