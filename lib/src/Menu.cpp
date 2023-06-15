#include "Menu.h"
#include "Text.h"
#include "Model.h"

Menu::Menu(Shader* shader, Shader* textShader) : hudShader(shader) {
    this->transform->setLocalPosition({ 0,0,-2.4 });
    this->transform->setLocalRotation({ 90,0,0 });

    enabledModel = new Model("res/models/HUD/player1_icon.obj");
    disabledModel = new Model("res/models/HUD/barBack.obj");

    activeButton = 1;

    button1 = new Entity(enabledModel, hudShader);
    button1->transform->setLocalPosition({0.2, 0, -0.4 });
    this->addChild(button1);

    button2 = new Entity(disabledModel, hudShader);
    button2->transform->setLocalPosition({ 0.2, 0, 0 });
    this->addChild(button2);

    startText = new Text(textShader, "res/font/arial.ttf");
    exitText = new Text(textShader, "res/font/arial.ttf");

    this->addChild(button1);
    this->addChild(button2);
}

void Menu::renderEntity() {
    switch(activeButton)
    {
        case 0:
            return;
        case 1:
            button1->model = enabledModel;
            button2->model = disabledModel;
            break;
        case 2:
            button1->model = disabledModel;
            button2->model = enabledModel;
            break;
        default:
            break;
    }

    Entity::renderEntity();
    startText->RenderText("Start", buttonsRegionLeft, exitRegionBottom, 2, glm::vec3(1));
    exitText->RenderText("Exit", buttonsRegionLeft, startRegionBottom, 2, glm::vec3(1));
}

void Menu::setActiveButton(int NewActiveButton) {
    activeButton = NewActiveButton;
}

void Menu::setResize(float newResizeX, float newResizeY)
{
    resizeX = newResizeX;
    resizeY = newResizeY;

    buttonsRegionLeft = resizeX * 800;
    buttonsRegionRight = resizeX * 1000;

    startRegionTop = resizeY * 200;
    startRegionBottom = resizeY * 300;

    exitRegionTop = resizeY * 350;
    exitRegionBottom = resizeY * 450;
}

void Menu::arrowInput(const int input /* 1 == up, -1 == down */) {
    activeButton += input;

    if(activeButton < 1) {
        activeButton = 2;
        return;
    }
    if(activeButton > 2){
        activeButton = 1;
        return;
    }
}

int Menu::getActiveButton() const {
    return activeButton;
}

void Menu::setCursorPos(double x, double y){
    cursorX = (float)x;
    cursorY = (float)y;
}

bool Menu::isCursorOnButtons() {
    if (cursorX < buttonsRegionLeft || cursorX > buttonsRegionRight || activeButton == 0)
        return false;

    if (cursorY >= startRegionTop && cursorY <= startRegionBottom) {
        activeButton = 1;
        return true;
    }
    if (cursorY >= exitRegionTop && cursorY <= exitRegionBottom) {
        activeButton = 2;
        return true;
    }

    return false;
}
