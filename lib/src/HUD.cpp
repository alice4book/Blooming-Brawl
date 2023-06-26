#include "HUD.h"
#include "Model.h"
#include "Text.h"
#include "Clock.h"
#include "Audio.h"

HUD::HUD(Shader* shader, Shader* textShader)
	: hudShader(shader)
	, textShader(textShader)
	, tilesCount(0)
	, player1Tiles(0)
	, player2Tiles(0)
	, seedsCount1(0)
	, seedsCount2(0)
	, player1RoundsWon(0)
	, player2RoundsWon(0)
{
	audio = new Audio(this);
	this->addComponent(audio);

	this->transform->setLocalPosition({ 0,0,-2.4 });
	this->transform->setLocalRotation({ 90,0,0 });

	hudIcon1 = new Entity("res/interface/player1_icon.obj", hudShader);
	hudIcon1->transform->setLocalPosition({ -1.4, 0, -0.7 });

	hudIcon2 = new Entity("res/interface/player2_icon.obj", hudShader);
	hudIcon2->transform->setLocalPosition({ 1.4, 0, -0.7 });

	barBack = new Entity("res/interface/barBack.obj", hudShader);
	barBack->transform->setLocalRotation({ 0, 90, 0 });
	barBack->transform->setLocalPosition({ 0, 0, -0.84 });
	barBack->transform->scaleEntity({ 1, 1, 2050 });

	bar1 = new Entity("res/interface/bar1.obj", hudShader);
	bar1->transform->setLocalRotation({ 0, -90, 0 });
	bar1->transform->setLocalPosition({ -1.1, 0, -0.84 });

	bar2 = new Entity("res/interface/bar2.obj", hudShader);
	bar2->transform->setLocalRotation({ 0, 90, 0 });
	bar2->transform->setLocalPosition({ 1.1, 0, -0.84 });

	bar2 = new Entity("res/interface/bar2.obj", hudShader);
	bar2->transform->setLocalRotation({ 0, 90, 0 });
	bar2->transform->setLocalPosition({ 1.1, 0, -0.84 });

	scoreBaner = new Entity("res/interface/score_baner.obj", hudShader);
	scoreBaner->transform->setLocalPosition({ 0,0,-2.4 });
	scoreBaner->transform->setLocalRotation({ 90, 0, 0 });
	scoreBaner->transform->addToLocalPosition({ 0, 0, -0.7 });

	seeds1 = new Text(textShader, "res/font/arial.ttf");
	seeds2 = new Text(textShader, "res/font/arial.ttf");
	timer = new Text(textShader, "res/font/arial.ttf");
	score1 = new Text(textShader, "res/font/arial.ttf");
	score2 = new Text(textShader, "res/font/arial.ttf");
	roundsWon1= new Text(textShader, "res/font/arial.ttf");
	roundsWon2 = new Text(textShader, "res/font/arial.ttf");

	this->addChild(hudIcon1);
	this->addChild(hudIcon2);
	this->addChild(barBack);
	this->addChild(bar1);
	this->addChild(bar2);
    hideHUD = false;
	showBanner = false;

	mapNr = 0;
}

void HUD::barSize(int player1, int player2)
{
	if (showBanner)
		return;
	player1Tiles = player1;
	player2Tiles = player2;
	if (tilesCount > 0) {
		bar1->transform->scaleEntity({ 1, 1, player1 * (BAR_SIZE / tilesCount) });
		bar2->transform->scaleEntity({ 1, 1, player2 * (BAR_SIZE / tilesCount) });
	}
}

void HUD::setSeedCount(int seeds1, int seeds2)
{
	seedsCount1 = seeds1;
	seedsCount2 = seeds2;
}

void HUD::setTilesCount(int newTilesCount) 
{
	tilesCount = newTilesCount;
}

void HUD::setResize(float newResizeX, float newResizeY)
{
	resizeX = newResizeX;
	resizeY = newResizeY;
}

void HUD::renderEntity() 
{
    if(hideHUD)
        return;
	Entity::renderEntity();
	timer->RenderText(std::to_string(clock->getSeconds()), resizeX * 600, resizeY * 600, resizeX, timerColor);
	if (showBanner) {
		scoreBaner->renderEntity(hudShader);
		score1->RenderText(std::to_string(player1Tiles) , resizeX * 400, resizeY * 340, resizeX * 1.5, { 0.f,1.f,1.f });
		score2->RenderText(std::to_string(player2Tiles) , resizeX * 840, resizeY * 340, resizeX * 1.5, { 0.f,1.f,1.f });
		roundsWon1->RenderText(std::to_string(player1RoundsWon), resizeX * 595, resizeY * 325, resizeX, { 0.f,1.f,1.f });
		roundsWon2->RenderText(std::to_string(player2RoundsWon), resizeX * 680, resizeY * 325, resizeX, { 0.f,1.f,1.f });
	}
	else {
		seeds1->RenderText(std::to_string(seedsCount1) + "/15", resizeX * 10, resizeY * 670, 0.8 * resizeX, {1.f,1.f,1.f});
		seeds2->RenderText(std::to_string(seedsCount2) + "/15", resizeX * 1190, resizeY * 670, 0.8 * resizeX, { 1.f,1.f,1.f });
	}
}

void HUD::nextMap()
{
	if(mapNr < 5)
		mapNr++;
}

void HUD::resetHUD()
{
	hideHUD = true;
	mapNr = 0;
	seedsCount1 = 5;
	seedsCount2 = 5;
	tilesCount = 0;
	player1Tiles = 0;
	player2Tiles = 0;
	player1RoundsWon = 0;
	player2RoundsWon = 0;
}

void HUD::setShowBanner(bool value)
{
	showBanner = value;
}

bool HUD::getShowBanner()
{
	return showBanner;
}

void HUD::decideWinner()
{
	if (player1Tiles == player2Tiles) {
		scoreBaner->model = &winningBanners[0];
		player1RoundsWon++;
		player2RoundsWon++;
	}
	else
		if (player1Tiles > player2Tiles) {
			scoreBaner->model = &winningBanners[1];
			player1RoundsWon++;
		}
		else {
			scoreBaner->model = &winningBanners[2];
			player2RoundsWon++;
		}
	
}

void HUD::makeBeep()
{
	audio->playMusic("res/audio/bleep.wav");
}

void HUD::changeTimerColor(glm::vec3 newTimerColor)
{
	timerColor = newTimerColor;
}

int HUD::currentMap()
{
	return mapNr;
}

void HUD::setHideHud(bool newHideHud) {
    hideHUD = newHideHud;
}


