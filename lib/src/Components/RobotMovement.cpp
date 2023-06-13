#include "RobotMovement.h"

#include "imgui_internal.h"

#include "DynamicColliderComponent.h"
#include "StaticColliderComponent.h"
#include "Transform.h"
#include "TimeManager.h"
#include <vector>
#include "Map.h"
#include "PlayerMovement.h"

#include <iostream> //usuñ

#include <TileStateType.h>
#include <algorithm>

std::vector<Node*> RobotMovement::findPath(Node* node)
{
	std::vector<Node*> nodes;
	if (!node->isPassable) return nodes;
	glm::vec2 snappedPos = getSnappedPosition();
	nodes = pathFinding.findPath(snappedPos.x, snappedPos.y, node->pos.x, node->pos.y);
	return nodes;
}

bool RobotMovement::moveToPoint(Node* node)
{
	if (node->pos == getSnappedPosition()) return true;
	std::vector<Node*> nodes = findPath(node);
	if (nodes.size() == 0) return false;
	for (int i = 0; i < nodes.size(); i++)
	{
		newPositions.push(glm::vec2(nodes[i]->pos.x * TILE_SIZE, nodes[i]->pos.y * TILE_SIZE));
	}
	return true;
}

void RobotMovement::rotate(glm::vec2 oldPos, glm::vec2 newPos)
{
	glm::vec3 forwardHere = { oldPos.x - newPos.x, 0.0f, oldPos.y - newPos.y };
	forwardHere = normalize(forwardHere);
	float angle = atan2(startForward.x * forwardHere.z - forwardHere.x * startForward.z,
		forwardHere.x * forwardHere.z + startForward.x * startForward.z) * (180.0 / 3.1415);
	if (angle == 0 || angle == -0) { angle = 0; }
	if (startForward == forwardHere) { angle = 180; }
	if (angle < 0 && angle > -180) { angle = -180 - angle; }
	transform->setLocalRotation({ 0,angle,0 });
}
	
glm::vec2 RobotMovement::getSnappedPosition()
{
	glm::vec3 localPos = transform->getLocalPosition();
	return glm::vec2(round(localPos.x/TILE_SIZE), round(localPos.z/TILE_SIZE));
}

RobotMovement::RobotMovement(Entity* parent, Transform* transform,
	DynamicColliderComponent* colliderBody, DynamicColliderComponent* colliderFront,
	float speed, PathFinding& pathFinding, float TILE_SIZE, glm::vec3 forward, float offset)
	: Component(parent)
	, forward(forward)
	, speed(speed)
	, transform(transform)
	, colliderBody(colliderBody)
	, colliderFront(colliderFront)
	, offset(offset)
	, pathFinding(pathFinding)
	, TILE_SIZE(TILE_SIZE)
	, isWondering(false)
	{
    timeManager = TimeManager::getInstance();
    timeManager->attach120FPS(this);
	startForward = forward;
	height = transform->getLocalPosition().y;

	colliderFront->setCenterOffset(glm::vec2(forward.x * offset, forward.z * offset));

}

void RobotMovement::update() {
	/*
	transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));

    if (!colliderFront->getTouchingDynamicComponents().empty()) {
        (this->*moveRob)(timeManager->getDeltaTime120FPS());
        return;
    }

    if (!colliderFront->getTouchingStaticComponents().empty())
    for (StaticColliderComponent* staticComp : colliderFront->getTouchingStaticComponents()) {
        if (!staticComp->getIsPassable()) {
            (this->*moveRob)(timeManager->getDeltaTime120FPS());
            break;
        }
    }*/
	//std::cout << "pos:" << getSnappedPosition().x << getSnappedPosition().y << std::endl;//usun¹æ
	if (newPositions.size() != 0)
	{
		if (isWondering) {
			findClosestNode();
		}
		float step = speed * timeManager->getDeltaTime120FPS();
		glm::vec3 currentPosition = transform->getLocalPosition();
		glm::vec2 newPosition = newPositions.front();
		glm::vec3 newPosition3D(newPosition.x, height, newPosition.y);
		glm::vec3 direction = newPosition3D - currentPosition;
		if (direction.x != 0 || direction.z != 0)
		{
			direction = glm::normalize(direction);
			transform->addToLocalPosition(direction * step);
		}

		if (glm::length(transform->getLocalPosition() - newPosition3D) < MIN_DISTANCE)
		{
			glm::vec2 snappedPos = getSnappedPosition();
			TileState* tileState = pathFinding.map->allTilesComp[(int)snappedPos.x][(int)snappedPos.y];

			tileState->changeTileState(EPlayerID::RobotDestroyer, EActionType::DestroyingFlower);
			player1->cancelAction(tileState);
			player2->cancelAction(tileState);

			glm::vec2 oldPos = newPositions.front();
			newPositions.pop();
			if (newPositions.size() > 0)
			{
				//std::cout << "Rotate" << std::endl;
				rotate(oldPos, newPositions.front());
			}
			else
			{
				findClosestNode();
			}
		}
	}
	else if (findClosestNode()) {}
	else
		wonder();
}

bool RobotMovement::findClosestNode(EPlayerID playerID)
{
	glm::vec2 currentPos = getSnappedPosition();

	Map* map = pathFinding.map;
	int mapHeight = map->MAX_ROWS;
	int mapWidth = map->MAX_COLUMNS;
	Node* closestNode = NULL;
	int closestDistance = 0;
	bool firstFound = false;
	int sight = 0;
	
	//std::cout << "findClosestNode "<< playerID << std::endl;
	/*
	switch (alpha)
	{
	case 0:
		sight = 0;
		for (int i = currentPos.x; i < mapHeight; i++)
		{
			int sightLeft = std::max((int)currentPos.y - sight, 0);
			int sightRight = std::min((int)currentPos.y + sight, mapWidth);
			for (int j = sightLeft; j <= sightRight; j++)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
			sight++;
		}
		break;
	case 45:
		for (int i = currentPos.x; i < mapHeight; i++)
		{
			for (int j = currentPos.y; j < mapWidth; j++)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
		}
		break;
	case 90:
		sight = 0;
		for (int i = currentPos.y; i < mapWidth; i++)
		{
			int sightDown = std::max((int)currentPos.x - sight, 0);
			int sightUp= std::min((int)currentPos.x + sight, mapHeight);
			for (int j = sightDown; j <= sightUp; j++)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
			sight++;
		}
		break;
	case 135:
		for (int i = currentPos.x; i > 0; i--)
		{
			for (int j = currentPos.y; j < mapWidth; j++)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
		}
	case 180 :
		sight = 0;
		for (int i = currentPos.x; i > 0; i--)
		{
			int sightLeft = std::max((int)currentPos.y - i, 0);
			int sightRight = std::min((int)currentPos.y + i, mapWidth);
			for (int j = sightLeft; j <= sightRight; j++)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
			sight++;
		}
		break;
	case 225:
		for (int i = currentPos.x; i > 0; i--)
		{
			for (int j = currentPos.y; j > 0; j--)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
		}
	case 270:
		sight = 0;
		for (int i = currentPos.y; i > 0; i--)
		{
			int sightDown = std::max((int)currentPos.x - sight, 0);
			int sightUp = std::min((int)currentPos.x + sight, mapHeight);
			for (int j = sightDown; j <= sightUp; j++)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
			sight++;
		}
		break;
	case 315:
		for (int i = currentPos.x; i < mapHeight; i++)
		{
			for (int j = currentPos.y; j > 0; j--)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
		}
	default:
		break;
	}
	if (closestNode == NULL)
	{
		for (int i = 0; i < mapHeight; i++)
		{
			for (int j = 0; j > mapWidth; j++)
			{
				checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound);
			}
		}
	}
	*/
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			checkIfClosest(i, j, currentPos, map, closestDistance, closestNode, firstFound, playerID);
		}
	}
	if (closestNode) {
		//std::cout << "closest " << closestNode->pos.x << " " << closestNode->pos.y << std::endl;
		if (isWondering) {
			std::queue<glm::vec2> empty;
			std::swap(newPositions, empty);
		}
		isWondering = false;
		moveToPoint(closestNode);
		return true;
	}
	return false;
}

void RobotMovement::wonder()
{
	Map* map = pathFinding.map;
	int mapHeight = map->MAX_ROWS;
	int mapWidth = map->MAX_COLUMNS;
	int i, j;
	i = rand() % mapHeight;
	j = rand() % mapWidth;	
	if (map->allTilesComp[i][j] == nullptr)
		return;
	EState state = map->allTilesComp[i][j]->state;
	if (state == EState::Empty)
	{
		isWondering = true;
		moveToPoint(map->nodes[i][j]);
	}
}

void RobotMovement::setPlayers(PlayerMovement* player1, PlayerMovement* player2)
{
	this->player1 = player1;
	this->player2 = player2;
}


void RobotMovement::checkIfClosest(int i, int j, glm::vec2 currentPos, Map* map, int& closestDistance, Node*& closestNode, bool& firstFound, EPlayerID playerID)
{
	if (map->allTilesComp[i][j] == nullptr)
		return;
	EState state = map->allTilesComp[i][j]->state;
	if (playerID == Player1) {
		//std::cout << "Player1" << std::endl;
		//std::cout << "state " << state << std::endl;
		if (state == EState::Growing2 ||
			state == EState::Grown2)
		{
			int distance = glm::length(currentPos - glm::vec2(i, j));
			if ((distance < closestDistance) || !firstFound)
			{
				closestNode = map->nodes[i][j];
				closestDistance = distance;
				firstFound = true;
			}
		}
	}
	else if (playerID == Player2) {
		//std::cout << "Player2" << std::endl;
		if (state == EState::Growing ||
			state == EState::Grown)
		{
			int distance = glm::length(currentPos - glm::vec2(i, j));
			if ((distance < closestDistance) || !firstFound)
			{
				closestNode = map->nodes[i][j];
				closestDistance = distance;
				firstFound = true;
			}
		}
	}
	else {
		//std::cout << "PlayerNone" << std::endl;
		if (state == EState::Growing || state == EState::Growing2 ||
			state == EState::Grown || state == EState::Grown2)
		{
			int distance = glm::length(currentPos - glm::vec2(i, j));
			if ((distance < closestDistance) || !firstFound)
			{
				closestNode = map->nodes[i][j];
				closestDistance = distance;
				firstFound = true;
			}
		}
	}

}

/*
// turns robot right (only with right angle)
void RobotMovement::turnRight(float dTime) {
	side += 90.f;
	if(side > 270.f)
		side = 0.0f;
	//transform->addToLocalPosition(forward * -(speed * dTime));
	transform->rotateLocal(glm::vec3(0.0f, -90.0f, 0.0f));
	glm::vec3 front;
	front.x = cos(glm::radians(side));
	front.y = 0.f;
	front.z = sin(glm::radians(side));
	forward = glm::normalize(front);
	colliderFront->setCenterOffset(glm::vec2(forward.x * offset, forward.z * offset));
}

// turns robot left (only with right angle)
void RobotMovement::turnLeft(float dTime) {
	side -= 90.f;
	if (side < 0.f)
		side = 270.0f;
	//transform->addToLocalPosition(forward * -(speed * dTime));
	transform->rotateLocal(glm::vec3(0.0f, 90.0f, 0.0f));
	glm::vec3 front;
	front.x = cos(glm::radians(side));
	front.y = 0.f;
	front.z = sin(glm::radians(side));
	forward = glm::normalize(front);
	colliderFront->setCenterOffset(glm::vec2(forward.x * offset, forward.z * offset));
}

void RobotMovement::noMove(float dTime) {
	transform->addToLocalPosition(forward * -(speed * dTime));
}
*/