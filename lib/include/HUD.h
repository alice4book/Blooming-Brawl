#pragma once
#include "Entity.h"
class Shader;

class HUD : public Entity{

private:
	Entity* hudIcon1;
	Entity* hudIcon2;

	Entity* barBack;
	Entity* bar1;
	Entity* bar2;

	Shader* hudShader;
	int tilesCount;

public:
	HUD(Shader* shader);

	void barScale(int bar);

	//For first player`s bar type 1, for second player`s bar type 2
	void setTilesCount(int newTilesCount);


};
