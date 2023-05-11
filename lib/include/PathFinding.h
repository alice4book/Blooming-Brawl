#pragma once
#include "Node.h"
#include <vector>

class Map;

class PathFinding
{
private:
	std::vector<Node*> open, closed;
	Node *startNode, *goalNode;
	int getNeighbourDistance(Node* node1, Node* node2);
	std::vector<Node*> calculatePath();
public:
	Map* map;
	PathFinding(Map* map);
	std::vector<Node*> findPath(int startX, int startZ, int endX, int endZ);
	
};

