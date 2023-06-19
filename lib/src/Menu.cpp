#include "Menu.h"
#include "Model.h"

Menu::Menu(Shader* shader) : hudShader(shader) {
    this->transform->setLocalPosition({ 0,0,-2.4 });
    this->transform->setLocalRotation({ 90,0,0 });

    exitEnabledModel = new Model("res/interface/menu/back_to_desktop_chosen/back_to_desktop_chosen.obj");
    exitDisabledModel = new Model("res/interface/menu/back_to_desktop/back_to_desktop.obj");

    startEnabledModel = new Model("res/interface/menu/begin_chosen/begin_chosen.obj");
    startDisabledModel = new Model("res/interface/menu/begin/begin.obj");

    activeButton = 1;

    background = new Entity("res/interface/menu/background/background.obj", hudShader);
    background->transform->setLocalPosition({0, -10, 0 });
    this->addChild(background);

    title = new Entity("res/interface/menu/title/title.obj", hudShader);
    title->transform->setLocalPosition({0, -0., -0.5 });
    this->addChild(title);

    button1 = new Entity(startEnabledModel, hudShader);
    button1->transform->setLocalPosition({0, 0, 0.1 });
    this->addChild(button1);

    button2 = new Entity(exitDisabledModel, hudShader);
    button2->transform->setLocalPosition({ 0, -0., 0.4 });
    this->addChild(button2);

    bestBears = new Entity("res/interface/menu/best_bears/best_bears.obj", hudShader);
    bestBears->transform->setLocalPosition({ 0, -0., 0.7 });
    this->addChild(bestBears);
}

void Menu::renderEntity() {
    switch(activeButton)
    {
        case 0:
            return;
        case 1:
            button1->model = startEnabledModel;
            button2->model = exitDisabledModel;
            break;
        case 2:
            button1->model = startDisabledModel;
            button2->model = exitEnabledModel;
            break;
        default:
            break;
    }

    Entity::renderEntity();
}

void Menu::setActiveButton(int NewActiveButton) {
    activeButton = NewActiveButton;
}

void Menu::setResize(float newResizeX, float newResizeY)
{
    resizeX = newResizeX;
    resizeY = newResizeY;

    buttonsRegionLeft = resizeX * 350;
    buttonsRegionRight = resizeX * 950;

    startRegionTop = resizeY * 350;
    startRegionBottom = resizeY * 425;

    exitRegionTop = resizeY * 475;
    exitRegionBottom = resizeY * 550;
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
