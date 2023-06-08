#pragma once
#include "GLFW/glfw3.h"
#include <string>

class World;
class Entity;
class Shader;
class Map;
class Model;
class PathFinding;

class Round {
private:
	World* skybox;

	Entity* mapManager;
	Entity* player1;
	Entity* player2;
	Entity* robot;

	Map* map;

	float TILE_SIZE = 0.254f;
public:
	Round(GLFWwindow* window, Model* tileModels, std::string* mapFiles, Shader* directionalShader,
		Shader* pickupShader, Shader* highlightShader);

	Entity* getPlayer1();
	Entity* getPlayer2();
	Entity* getRobot();
	Entity* getMapManager();
	Map* getMap();
};