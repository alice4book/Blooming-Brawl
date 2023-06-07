#ifndef OPENGLGP_MENU_H
#define OPENGLGP_MENU_H


#include "Entity.h"

class Text;

class Menu : public Entity {
private:
    Entity* button1;
    Entity* button2;

    Model* enabledModel;
    Model* disabledModel;

    Shader* textShader;
    Shader* hudShader;

    Text* score1;
    Text* score2;

    // 0 == no menu
    int activeButton;

public:
    Menu(Shader* shader, Shader* textShader);

    void renderEntity();

    void setActiveButton(int NewActiveButton);
};


#endif //OPENGLGP_MENU_H
