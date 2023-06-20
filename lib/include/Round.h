#pragma once
#include "GLFW/glfw3.h"
#include <string>

//WIP

class World;
class Entity;
class Shader;
class Map;
class Model;
class PathFinding;
class HUD;

class Round {
private:
	Entity* round;

	Map* maps[5];
	Entity* mapManagers[5];

	Map* currentMap;
	Entity* currentMapManager;

	Entity* player1;
	Entity* player2;
	Entity* robot;
	Entity* allTools[2];

	HUD* hud;

	int roundTime;
	int currentToolSize;
	float TILE_SIZE = 0.254f;
public:
	Round(GLFWwindow* window, Model* tileModels, std::string* mapFiles, Shader* directionalShader,
		Shader* pickupShader, Shader* highlightShader, HUD* hud);

	void changeRound(int roundNr);
	void regenerateMaps(Model* tileModels, std::string* mapFiles, Shader* directionalShader,
		Shader* pickupShader, Shader* highlightShader);

	Entity* getPlayer1();
	Entity* getPlayer2();
	Entity* getRobot();
	Entity* getMapManager();
	Map* getMap();
	Entity** getTools();
	int getToolsSize();
};