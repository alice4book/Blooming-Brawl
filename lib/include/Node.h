#pragma once
#include "glm/vec2.hpp"

class Node
{
public:
	bool isPassable = true;
	glm::vec2 pos = { 0,0 };
	int G = 0, H = 0, F = 0;
	Node* neighbours[8] = {};
	Node* cameFromNode = NULL;
};

