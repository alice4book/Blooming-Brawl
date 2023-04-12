#include "HUD.h"
#include "Transform.h"

HUD::HUD(Shader* shader) 
	: hudShader(shader)
	, tilesCount(10)
{
	this->transform->setLocalPosition({ 0,0,-2.4 });
	this->transform->setLocalRotation({ 90,0,0 });

	hudIcon1 = new Entity("res/models/HUD/player1_icon.obj", hudShader);
	hudIcon1->transform->setLocalPosition({ -1.4, 0, -0.7 });

	hudIcon2 = new Entity("res/models/HUD/player1_icon.obj", shader);
	hudIcon2->transform->setLocalPosition({ 1.4, 0, -0.7 });

	barBack = new Entity("res/models/HUD/barBack.obj", shader);
	barBack->transform->setLocalRotation({ 0, 90, 0 });
	barBack->transform->setLocalPosition({ 0, 0, -0.84 });
	//barBack->transform->scaleEntity({ 2, 2, 2 });

	bar1 = new Entity("res/models/HUD/bar.obj", shader);
	bar1->transform->setLocalRotation({ 0, -90, 0 });
	bar1->transform->setLocalPosition({ -1.1, 0, -0.84 });

	bar2 = new Entity("res/models/HUD/bar.obj", shader);
	bar2->transform->setLocalRotation({ 0, 90, 0 });
	bar2->transform->setLocalPosition({ 1.1, 0, -0.84 });


	this->addChild(hudIcon1);
	this->addChild(hudIcon2);
	this->addChild(barBack);
	this->addChild(bar1);
	this->addChild(bar2);
}

void HUD::barScale(int bar) 
{
	if (tilesCount != 0) {
		if(bar == 1){
			bar1->transform->addToScaleEntity({ 0, 0, 2.2f / tilesCount });
		}
		else if (bar == 2) {
			bar2->transform->addToScaleEntity({ 0, 0, 2.2f / tilesCount });
		}
	}

}

void HUD::setTilesCount(int newTilesCount) 
{
	tilesCount = newTilesCount;
}