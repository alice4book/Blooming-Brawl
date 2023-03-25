#include "RobotMovement.h"
#include "Transform.h"

RobotMovement::RobotMovement(Entity* parent, Transform* transform, float speed) 
	: Component(parent),
	forward(glm::vec3(1.0f, 0.0f, 0.0f)), 
	speed(speed),
	transform(transform) {}

void RobotMovement::update() {
	transform->addToLocalPosition(forward * speed);
}
