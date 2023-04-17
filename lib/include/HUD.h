#pragma once
#include "Entity.h"
class Shader;

class HUD : public Entity{

private:
	int BACKBAR_SIZE = 2050;
	int BAR_SIZE = 2200;

	Entity* hudIcon1;
	Entity* hudIcon2;

	Entity* barBack;
	Entity* bar1;
	Entity* bar2;

	Shader* hudShader;
	int tilesCount;
	int player1Tiles;
	int player2Tiles;

public:
	HUD(Shader* shader);

	//Size bars acording to score
	void barSize(int player1, int player2);

	void setTilesCount(int newTilesCount);
};
