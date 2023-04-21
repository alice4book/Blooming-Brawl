#pragma once
#include "Entity.h"
#include "Shader.h"
#include "Text.h"

class HUD : public Entity{

private:
	int BACKBAR_SIZE = 2050;
	int BAR_SIZE = 2200;

	Entity* hudIcon1;
	Entity* hudIcon2;

	Entity* barBack;
	Entity* bar1;
	Entity* bar2;

	//std::string str_score1;
	//std::string str_score2;
	std::string str_timer;

	Text* score1;
	Text* score2;
	Text* timer;

	Shader* textShader;
	Shader* hudShader;

	int tilesCount;
	int player1Tiles;
	int player2Tiles;

public:
	HUD(Shader* shader, Shader* textShader);

	//Size bars acording to score
	void barSize(int player1, int player2);

	void setTilesCount(int newTilesCount);

	void renderEntity();
};
