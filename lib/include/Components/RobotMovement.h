#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "RobotMovementType.h"
#include "PlayerIDType.h"
#include <queue>
#include "PathFinding.h"

class Transform;
class DynamicColliderComponent;
class TimeManager;

class RobotMovement : public Component{
private:
	glm::vec3 forward;
	glm::vec3 startForward;

	float side;
	float speed = 0.005f;
	Transform* transform;
	DynamicColliderComponent* colliderBody;
	DynamicColliderComponent* colliderFront;
	float offset;
	//Tells what kind of movement
	RobotMovementType moveType = eStop;
	void (RobotMovement::*moveRob)(float);
    TimeManager* timeManager;

	float TILE_SIZE;;
	std::queue<glm::vec2> newPositions;
	PathFinding pathFinding;
	float height;
	float MIN_DISTANCE = 0.08;

	std::vector<Node*> findPath(Node* node);
	bool moveToPoint(Node* node);
	void rotate(glm::vec2 oldPos, glm::vec2 newPos);
	glm::vec2 getSnappedPosition();
	int alpha = 0;
	void checkIfClosest(int x, int y, glm::vec2 currentPos, Map* map, int& closestDistance, Node*& closestNode, bool& firstFound, EPlayerID playerID);

	bool isWondering;

public:
	RobotMovement(Entity* parent, Transform* transform, DynamicColliderComponent* colliderBody,
		DynamicColliderComponent* colliderFront, float speed, 
		RobotMovementType type, PathFinding& pathFinding, float TILE_SIZE, glm::vec3 forward = { 1, 0, 0 }, float offset = 0.04f );
	//void turnRight(float dTime);
	//void turnLeft(float dTime);	
	//void noMove(float dTime);
	void update() override;
	bool findClosestNode(EPlayerID playerID = None);
	//bool findClosestNode();
	//WIP
	void wonder();
};