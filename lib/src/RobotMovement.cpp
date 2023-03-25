#include "RobotMovement.h"
#include "Transform.h"

RobotMovement::RobotMovement(Entity* parent, Transform* transform, float speed, glm::vec3 forward)
	: Component(parent),
	forward(forward),
	speed(speed),
	transform(transform) {}

void RobotMovement::update() {
	transform->addToLocalPosition(forward * speed);
}
