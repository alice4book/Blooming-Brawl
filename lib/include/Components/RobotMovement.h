#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "PlayerIDType.h"
#include <queue>
#include "PathFinding.h"

class Transform;
class DynamicColliderComponent;
class TimeManager;
class PlayerMovement;
class Audio;

class RobotMovement : public Component{
private:
	glm::vec3 forward;
	glm::vec3 startForward;

	float side;
	float speed = 0.002f;
	Transform* transform;
	DynamicColliderComponent* colliderBody;
	DynamicColliderComponent* colliderFront;
	float offset;
	//Tells what kind of movement
	void (RobotMovement::*moveRob)(float);
    TimeManager* timeManager;
	Audio* audio;

	float TILE_SIZE;
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

	PlayerMovement* player1, *player2;

public:
	RobotMovement(Entity* parent, Transform* transform, DynamicColliderComponent* colliderBody,
		DynamicColliderComponent* colliderFront, float speed, PathFinding& pathFinding, 
		float TILE_SIZE, glm::vec3 forward = { 1, 0, 0 }, float offset = 0.04f);

	void update() override;
	void enable(bool value) override;
	bool findClosestNode(EPlayerID playerID = None);

	void wonder();
	void setPlayers(PlayerMovement* player1, PlayerMovement* player2);
	void clearPath();
	void setPathFinding(PathFinding& newPathFinding);
};