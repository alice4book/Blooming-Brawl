#include "HUD.h"
#include "Transform.h"

HUD::HUD(Shader* shader) 
	: hudShader(shader)
	, tilesCount(0)
	, player1Tiles(0)
	, player2Tiles(0)
{
	this->transform->setLocalPosition({ 0,0,-2.4 });
	this->transform->setLocalRotation({ 90,0,0 });

	hudIcon1 = new Entity("res/models/HUD/player1_icon.obj", hudShader);
	hudIcon1->transform->setLocalPosition({ -1.4, 0, -0.7 });

	hudIcon2 = new Entity("res/models/HUD/player1_icon.obj", hudShader);
	hudIcon2->transform->setLocalPosition({ 1.4, 0, -0.7 });

	barBack = new Entity("res/models/HUD/barBack.obj", hudShader);
	barBack->transform->setLocalRotation({ 0, 90, 0 });
	barBack->transform->setLocalPosition({ 0, 0, -0.84 });
	barBack->transform->scaleEntity({ 1, 1, 2050 });

	bar1 = new Entity("res/models/HUD/bar1.obj", hudShader);
	bar1->transform->setLocalRotation({ 0, -90, 0 });
	bar1->transform->setLocalPosition({ -1.1, 0, -0.84 });

	bar2 = new Entity("res/models/HUD/bar2.obj", hudShader);
	bar2->transform->setLocalRotation({ 0, 90, 0 });
	bar2->transform->setLocalPosition({ 1.1, 0, -0.84 });

	this->addChild(hudIcon1);
	this->addChild(hudIcon2);
	this->addChild(barBack);
	this->addChild(bar1);
	this->addChild(bar2);
}

void HUD::barSize(int player1, int player2)
{
	if (tilesCount > 0) {
		bar1->transform->scaleEntity({ 1, 1, player1 * (BAR_SIZE / tilesCount) });
		bar2->transform->scaleEntity({ 1, 1, player2 * (BAR_SIZE / tilesCount) });
	}
}

void HUD::setTilesCount(int newTilesCount) 
{
	tilesCount = newTilesCount;
}