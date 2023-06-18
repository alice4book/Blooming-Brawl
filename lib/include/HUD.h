#pragma once
#include "Entity.h"

class Shader;
class Text;
class TimerManager;
class Clock;

class HUD : public Entity{

private:
	double BACKBAR_SIZE = 2050;
	double BAR_SIZE = 2200;

	float resizeX = 1;
	float resizeY = 1;

	Entity* hudIcon1;
	Entity* hudIcon2;

	Entity* barBack;
	Entity* bar1;
	Entity* bar2;

	std::string str_timer;

	Text* seeds1;
	Text* seeds2;
	Text* timer;

	Shader* textShader;
	Shader* hudShader;

	int tilesCount;
	int player1Tiles;
	int player2Tiles;

	int seedsCount1;
	int seedsCount2;

	int mapNr;

    bool hideHUD;

public:
	Clock* clock;

    void setHideHud(bool newHideHud);

	HUD(Shader* shader, Shader* textShader);

	//Size bars acording to score
	void barSize(int player1, int player2);

	void setSeedCount(int seeds1, int seeds2);

	void setTilesCount(int newTilesCount);

	void setResize(float newResizeX, float newResizeY);

	void renderEntity();

	int currentMap();

	void nextMap();
};
