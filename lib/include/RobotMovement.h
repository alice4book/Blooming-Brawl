#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform;

class RobotMovement : public Component{
private:
	glm::vec3 forward;
	float speed;
	Transform* transform;
	
public:
	RobotMovement(Entity* parent, Transform* transform, float speed, glm::vec3 forward = {1, 0, 0});
	void update() override;

};