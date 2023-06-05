#include "Menu.h"
#include "Text.h"
#include "Model.h"

Menu::Menu(Shader* shader, Shader* textShader) : hudShader(shader), textShader(textShader) {
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

    score1 = new Text(textShader, "res/font/arial.ttf");
    score2 = new Text(textShader, "res/font/arial.ttf");

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
    score1->RenderText("Start", 800, 450, 2, glm::vec3(1));
    score2->RenderText("Exit", 800, 300, 2, glm::vec3(1));
}

void Menu::setActiveButton(int NewActiveButton) {
    activeButton = NewActiveButton;
}
