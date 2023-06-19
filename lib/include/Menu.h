#ifndef OPENGLGP_MENU_H
#define OPENGLGP_MENU_H


#include "Entity.h"

class Text;

class Menu : public Entity {
private:
    Entity* background;
    Entity* title;
    Entity* button1;
    Entity* button2;
    Entity* bestBears;

    Model* exitEnabledModel;
    Model* exitDisabledModel;

    Model* startEnabledModel;
    Model* startDisabledModel;

    Shader* hudShader;

    float cursorX{}, cursorY{};

    float buttonsRegionLeft{};
    float buttonsRegionRight{};

    float startRegionTop{};
    float startRegionBottom{};

    float exitRegionTop{};
    float exitRegionBottom{};

    float resizeX = 1.f, resizeY = 1.f;

    // 0 == no menu
    int activeButton;

public:
    explicit Menu(Shader* shader);

    void renderEntity() override;

    void setActiveButton(int NewActiveButton);

    void setResize(float newResizeX, float newResizeY);

    void arrowInput(int input);

    [[nodiscard]] int getActiveButton() const;

    void setCursorPos(double x, double y);

    bool isCursorOnButtons();
};


#endif //OPENGLGP_MENU_H
