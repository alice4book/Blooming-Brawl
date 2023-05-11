#include "PathFinding.h"
#include "Map.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>

int PathFinding::getNeighbourDistance(Node* node1, Node* node2)
{
    if (node1->pos.x == node2->pos.x || node1->pos.y == node2->pos.y) return 10;
    else return 14;
}

std::vector<Node*> PathFinding::calculatePath()
{
    std::vector<Node*> path;
    path.push_back(goalNode);
    Node* currentNode = goalNode;
    while (currentNode->cameFromNode != NULL)
    {
        std::cout << currentNode->cameFromNode->pos.x<<" "<< currentNode->cameFromNode->pos.y<<std::endl;
        path.push_back(currentNode->cameFromNode);
        currentNode = currentNode->cameFromNode;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

PathFinding::PathFinding(Map* map) : map(map)
{

}

std::vector<Node*> PathFinding::findPath(int startX, int startZ, int endX, int endZ)
{
    startNode = map->nodes[startX][startZ];
    goalNode = map->nodes[endX][endZ];
    open.clear();
    closed.clear();
    startNode->cameFromNode = NULL;
    open.push_back(startNode);
    while (open.size() != 0)
    {
        std::sort(open.begin(), open.end(), [](const Node* n1, const Node* n2) { return n1->F < n2->F; });
        Node* node = open[0];
        closed.push_back(node);
        open.erase(open.begin());
        if (node == goalNode)
        {
            return calculatePath();
        }
        for (int i = 0; i < 8; i++)
        {
            Node* neighbour = node->neighbours[i];
            if (!neighbour || !neighbour->isPassable) continue;
            if (std::find(closed.begin(), closed.end(), neighbour) != closed.end()) continue;

            int tentativeG = getNeighbourDistance(node, neighbour) + node->G;
            bool tentativeIsBetter = false;
            if (std::find(open.begin(), open.end(), neighbour) == open.end())
            {
                open.push_back(neighbour);
                neighbour->H = (int)(glm::length(neighbour->pos - goalNode->pos) * 10);
                tentativeIsBetter = true;
            }
            else if (tentativeG < neighbour->G)
            {
                tentativeIsBetter = true;
            }
            if (tentativeIsBetter)
            {
                neighbour->cameFromNode = node;
                neighbour->G = tentativeG;
                neighbour->F = neighbour->G + neighbour->H;
            }
        }
    }
    return std::vector<Node*>();
}
