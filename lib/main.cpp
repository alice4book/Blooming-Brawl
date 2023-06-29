#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include <iostream>
#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Audio.h"
#include "Shader.h"
#include "Camera.h"
#include "World.h"
#include "StaticColliderComponent.h"
#include "DynamicColliderComponent.h"

#include "Model.h"
#include "Map.h"
#include "HUD.h"
#include "Tool.h"

#include "TimeManager.h"
#include "Menu.h"
#include "Round.h"

#include "Animator.h"

#define GLFW_GAMEPAD_BUTTON_A 0
#define GLFW_GAMEPAD_BUTTON_B 1
#define GLFW_GAMEPAD_BUTTON_X 2
#define GLFW_GAMEPAD_BUTTON_Y 3

#define TILE_SIZE 0.254f

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void set_window_size_callback(GLFWwindow* window, int x, int y);

// camera
Camera camera(glm::vec3(0.0f, 3.f, 0.0f), glm::vec3(0, 1, 0), 0, 0);
float lastX;
float lastY;
bool firstMouse = true;

// timing
TimeManager* timeManager = TimeManager::getInstance();
bool click = false;

Menu* menu;

float angleSun = 0.0f;
int currentSecond = 0.0f;
int previousSecond = 0.0f;
int roundTime = 1.0f;

//gamepad
int axisCount;

struct WindowData {
    int resolutionX, resolutionY;
} windowData{1280, 720};

float resizeX = 1.f;
float resizeY = 1.f;


float gamma = 1;

int roundNr = 1;

bool executeMenu = false;
bool windowSizeChanged = false;

int main()
{
    // Setup window
    if (!glfwInit())
        return 1;

    std::srand(time(nullptr));

#pragma region GL_version
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
#pragma endregion

#pragma region resolution&monitor_settings
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    lastX = (float)mode->width / 2;
    lastY = (float)mode->height / 2;

    //    Fullscreen
    //    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Farm Engine", monitor, nullptr);

    //    Windowed mode
    GLFWwindow* window = glfwCreateWindow(windowData.resolutionX, windowData.resolutionY, "Farm Engine", nullptr, nullptr);

    glfwSetWindowUserPointer(window, (void*)&windowData);
    glfwSetWindowSizeCallback(window, set_window_size_callback);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
#pragma endregion

#pragma region various_settings
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    /*
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    */
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
#pragma endregion

#pragma region setup

    // build and compile our shader zprogram
    Shader modelShader("res/shaders/vertexModel.vert", "res/shaders/fragment.frag");
    Shader skyboxShader("res/shaders/vertexSkybox.vert", "res/shaders/fragmentSkybox.frag");
    Shader pickupShader("res/shaders/vertexPickUp.vert", "res/shaders/rimLight.frag");
    Shader highlightShader("res/shaders/vertexModel.vert", "res/shaders/highlightLight.frag");
    Shader directionalShader("res/shaders/vertexModel.vert", "res/shaders/directional.frag");
    Shader depthShader("res/shaders/depthShader.vert", "res/shaders/depthShader.frag");
    //    Shader blurShader("res/shaders/vertexModel.vert", "res/shaders/blur.frag");
    //    Shader ambientShader("res/shaders/vertexModel.vert", "res/shaders/ambientLight.frag");
    //    Shader reflectShader("res/shaders/vertexModel.vert", "res/shaders/reflect.frag");
    //    Shader phongBlinnShader("res/shaders/vertexModel.vert", "res/shaders/phongblinn.frag");
    //    Shader glassShader("res/shaders/glassShader.vert", "res/shaders/glassShader.frag");

    glm::vec3 dirLightColor(1.0f, 1.0f, 0.5f);

    World* skybox = World::getInstance();
    skybox->setShader(&skyboxShader);

    Model tileModels[8] = {
    Model("res/models/trawa.obj"),
    Model("res/models/maly_kwiat2.obj"),
    Model("res/models/maly_kwiat.obj"),
    Model("res/models/duzy_kwiat2.obj"),
    Model("res/models/duzy_kwiat.obj"),
    Model("res/models/skaly.obj"),
    Model("res/models/krzak.obj"),
    Model("res/models/spalony.obj")
    };

    std::string mapFiles[5] = {
    "res/maps/map1.txt",
    "res/maps/map2.txt",
    "res/maps/map3.txt",
    "res/maps/map4.txt",
    "res/maps/map5.txt"
    };

    Shader hudShader("res/shaders/HUD.vert", "res/shaders/HUD.frag");
    Shader textShader("res/shaders/text.vert", "res/shaders/text.frag");

    HUD hud(&hudShader, &textShader);
    hud.setHideHud(true);

    menu = new Menu(&hudShader);
    skybox->addChild(menu);
    menu->setResize(resizeX, resizeY);
    int menuActiveButton;

    Shader animationShader("res/shaders/basicAnim.vert", "res/shaders/directional.frag");

    Round round(window, tileModels, mapFiles, &directionalShader, &pickupShader, &highlightShader, &hud, &animationShader);

    camera.setCameraPosition(TILE_SIZE, 4.2f, round.getMap()->MAX_COLUMNS, round.getMap()->MAX_ROWS);

    glfwSetKeyCallback(window, key_callback);
#pragma endregion

#pragma region House
    Entity background("res/models/background_trawa.obj", &directionalShader);
    Entity house1("res/models/dom.obj", &directionalShader);
    Entity house2("res/models/dom_czerwony.obj", &directionalShader);
    Entity bush("res/models/drzewo.obj", &directionalShader);
    Entity bush1("res/models/drzewo.obj", &directionalShader);
    Entity bush2("res/models/drzewo.obj", &directionalShader);
    Entity bush3("res/models/drzewo.obj", &directionalShader);
    Entity bush4("res/models/drzewo.obj", &directionalShader);
    Entity bush5("res/models/drzewo.obj", &directionalShader);
    Entity bush6("res/models/drzewo.obj", &directionalShader);
    Entity fan("res/models/wiatrak.obj", &directionalShader);
    
    background.transform->setLocalPosition({2.2f,-0.5f,2.2f});
    house2.transform->setLocalRotation({0.f,180.f,0.f});
    house2.transform->setLocalPosition({ 2.5f,0.f, 4.8f});
    fan.transform->setLocalPosition({ 2.5f,-0.5f, -0.7f});
    bush.transform->setLocalPosition({ 2.5f,0.f, 4.8f});
    bush1.transform->setLocalPosition({ 2.8f,0.f, 4.0f});
    bush1.transform->setLocalRotation({ 0.0f, 65.f, 0.f});
    bush2.transform->setLocalPosition({ 2.8f,0.f, 3.2f});
    bush2.transform->setLocalRotation({ 0.f, 240.f, 0.f });
    bush3.transform->setLocalPosition({ 2.8f,0.f, 2.4f});
    bush3.transform->setLocalRotation({ 0.f, 170.f, 0.f });
    bush4.transform->setLocalPosition({ 2.8f,0.f, 1.6f});
    bush4.transform->setLocalRotation({ 0.f, 300.f, 0.f });
    bush5.transform->setLocalPosition({ 2.8f,0.f, 0.8f});
    bush5.transform->setLocalRotation({ 0.f, 280.f, 0.f });
    bush6.transform->setLocalPosition({ 2.8f,0.f, 0.0f});
    bush6.transform->setLocalRotation({ 0.f, 320.f, 0.f });
    background.transform->setLocalPosition({ 2.2f,-0.5f,2.2f });
    house2.transform->setLocalRotation({ 0.f,180.f,0.f });
    house2.transform->setLocalPosition({ 2.5f,0.f, 4.8f });

    skybox->addChild(&background);
    skybox->addChild(&house1);
    skybox->addChild(&house2);
    skybox->addChild(&bush);
    skybox->addChild(&bush1);
    skybox->addChild(&bush2);
    skybox->addChild(&bush3);
    skybox->addChild(&bush4);
    skybox->addChild(&bush5);
    skybox->addChild(&bush6);
    skybox->addChild(&fan);

#pragma endregion

#pragma region AnimationTest
    //Model animatedModel("res/animations/animacje_farmera/czerwony_farmer/farmer_czerwony_bieg.fbx");
    //Animation animation("res/animations/animacje_farmera/czerwony_farmer/farmer_czerwony_bieg.fbx", &animatedModel);
    //Model animatedModel("res/animated_models/ball.fbx");

    //Shader animationShader("res/shaders/vertexModel.vert", "res/shaders/directional.frag");
    //Entity animEntity("res/animated_models/ball.fbx", &animationShader);
    /*
    Animation animation("res/animated_models/ball.fbx", animEntity.model);
    Animator animator(&animation);
    timeManager->attachUnlimitedFPS(&animator);
    animEntity.addComponent(&animator);
    animEntity.setupAnimator();
    */
    //animEntity.transform->scaleEntity({ 0.0012f, .0012f, .0012f });
    //skybox->addChild(&animEntity);

#pragma endregion

#pragma region Power Up Setting
    float rimLight = 0.6f;
#pragma endregion

#pragma region Shadow
    // Framebuffer for shadow map
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float near_plane = 1.0f, far_plane = 100.0f;
    glm::mat4 orthogonalProjection = glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(glm::vec3(1.0f, 2.0f, 0.0f), //eye
        glm::vec3(2.0f, 0.0f, 3.0f), //center
        glm::vec3(0.0f, 1.0f, 0.0f)); //up (y)
    glm::mat4 lightProjection = orthogonalProjection * lightView;

    //glEnable(GL_FRAMEBUFFER_SRGB);

#pragma endregion

#pragma region Audio   
    Audio audioBackground(skybox);
    skybox->addComponent(&audioBackground);
    audioBackground.playBackgroundMusic("res/audio/x.wav");
#pragma endregion


    int currentRound = 0;

    roundTime = round.getRoundTime();
    angleSun = 5.0f / round.getRoundTime(); // *M_PI / 180;
    previousSecond = round.getRoundTime();
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        if (windowSizeChanged) {
            windowSizeChanged = false;
            hud.setResize(resizeX, resizeY);
            menu->setResize(resizeX, resizeY);
        }

        if (currentRound < 4 && currentRound != hud.currentMap()) {
            round.changeRound(hud.currentMap());
            currentRound = hud.currentMap();

            lightView = glm::lookAt(glm::vec3(1.0f, 2.0f, 0.0f), //eye
                glm::vec3(2.0f, 0.0f, 3.0f), //center
                glm::vec3(0.0f, 1.0f, 0.0f)); //up (y)
            dirLightColor = glm::vec3(1.0f, 1.0f, 0.5f);
            previousSecond = round.getRoundTime();

        }
        else if (hud.currentMap() == 5) {
            currentRound = 0;
            executeMenu = false;
            menu->setActiveButton(1);
            hud.resetHUD();
            round.regenerateMaps(tileModels, mapFiles, &directionalShader, &pickupShader, &highlightShader);
            camera.setCameraRotation(0, 0.f);
        }

        menuActiveButton = menu->getActiveButton();

        // You are in menu and you pressed Enter
        if (executeMenu) {
            executeMenu = false;

            switch (menuActiveButton) {
            case 1:
                // start the game
                menu->setActiveButton(0);
                hud.setHideHud(false);
                camera.setCameraRotation(0, -60.f);
                glfwSetKeyCallback(window, nullptr);
                break;
            case 2:
                // end the game
                glfwDestroyWindow(window);
                glfwTerminate();
                exit(EXIT_SUCCESS);
            default:
                break;
            }
        }

        // Menu has no button chosen, so it's inactive
        if (menuActiveButton == 0) {
            processInput(window);
            timeManager->updateTime();
        }
        else {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            menu->setCursorPos(x, y);
            menu->isCursorOnButtons();
        }


        glfwPollEvents();
        //std::cout << cos((360 / 120)) * timeManager->getDeltaTime120FPS() << std::endl;
        //std::cout << round.getRoundTime() << std::endl;
        //std::cout << round.getClockSeconds() << std::endl;
        currentSecond = round.getClockSeconds();
        if (previousSecond > currentSecond) {
            previousSecond = currentSecond;
            lightView = glm::lookAt(glm::vec3(1.0f, 2.0f, angleSun * (roundTime - currentSecond)), //eye
                glm::vec3(2.0f, 0.0f, 3.0f), //center
                glm::vec3(0.0f, 1.0f, 0.0f)); //up (y)
            lightProjection = orthogonalProjection * lightView;
            dirLightColor.r -= 1.0f / roundTime;
            dirLightColor.g -= 1.0f / roundTime;
            std::cout << dirLightColor.r << std::endl;
        }



        // input
        processInput(window);

        //shadow
        glEnable(GL_DEPTH_TEST);

        depthShader.use();
        depthShader.setMat4("lightProjection", lightProjection);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        if (round.getRobot() != nullptr) {
            round.getRobot()->renderEntity(&depthShader);
        }

        //round.getPlayer1()->renderEntity(&depthShader);
        //round.getPlayer2()->renderEntity(&depthShader);
        for (int i = 0; i < round.getMap()->getShadowTiles().size(); i++) {
            round.getMap()->getShadowTiles()[i]->renderEntity(&depthShader);
        }
        for (int i = 0; i < round.getMap()->getPlayerShadowTiles().size(); i++) {
            round.getMap()->getPlayerShadowTiles()[i]->renderEntity(&depthShader);
        }
        for (int i = 0; i < round.getToolsSize(); i++) {
            if (round.getTools()[i] != nullptr) {
                round.getTools()[i]->renderEntity(&depthShader);
            }
        }
        bush.renderEntity(&depthShader);
        bush1.renderEntity(&depthShader);
        bush2.renderEntity(&depthShader);
        bush3.renderEntity(&depthShader);
        bush4.renderEntity(&depthShader);
        bush5.renderEntity(&depthShader);
        bush6.renderEntity(&depthShader);
        fan.renderEntity(&depthShader);
        house1.renderEntity(&depthShader);
        house2.renderEntity(&depthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, windowData.resolutionX, windowData.resolutionY);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // orthographic
//         constexpr float size = 2.f;
//         float aspectRatio = (float) windowData.resolutionX / (float) windowData.resolutionY;
//         glm::mat4 projection = glm::ortho(aspectRatio * -size, aspectRatio * size,  -size,  size, 0.1f, 100.f);

        // perspective
        glm::mat4 projection;
        if (windowData.resolutionX != 0 && windowData.resolutionY != 0) {
            projection = glm::perspective(glm::radians(camera.Zoom), (float)windowData.resolutionX / (float)windowData.resolutionY, 0.1f, 100.0f);
        }
        glm::mat4 view = camera.GetViewMatrix();



        // activate shader
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setVec3("viewPos", camera.Position);

        pickupShader.use();
        pickupShader.setMat4("projection", projection);
        pickupShader.setMat4("view", view);
        pickupShader.setVec3("viewPos", camera.Position);
        pickupShader.setFloat("n_r", rimLight);

        highlightShader.use();
        highlightShader.setMat4("projection", projection);
        highlightShader.setMat4("view", view);
        highlightShader.setVec3("viewPos", camera.Position);
        highlightShader.setVec3("hlcolor", { 0.01,0.02,0.01 });

        animationShader.use();
        animationShader.setMat4("projection", projection);
        animationShader.setMat4("view", view);

        animationShader.setVec4("aColor", glm::vec4(dirLightColor, 0.0f));
        animationShader.setMat4("lightProjection", lightProjection);
        glUniform1i(glGetUniformLocation(depthShader.ID, "depthMap"), 1);
        animationShader.setMat4("lightProjection", lightProjection);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        animationShader.setInt("texture_diffuse1", 0);
        glUniform1i(glGetUniformLocation(animationShader.ID, "depthMap"), 2);
        animationShader.setFloat("gamma", gamma);
        animationShader.setVec3("dirLightColor", (dirLightColor));
        animationShader.setVec3("viewPos", camera.Position);


        directionalShader.use();
        glUniform1i(glGetUniformLocation(depthShader.ID, "depthMap"), 1);
        directionalShader.setMat4("lightProjection", lightProjection);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        directionalShader.setInt("texture_diffuse1", 0);
        glUniform1i(glGetUniformLocation(directionalShader.ID, "depthMap"), 2);
        directionalShader.setFloat("gamma", gamma);
        directionalShader.setMat4("projection", projection);
        directionalShader.setMat4("view", view);
        directionalShader.setVec4("aColor", glm::vec4(dirLightColor, 0.0f));
        directionalShader.setVec3("dirLightColor", (dirLightColor));
        directionalShader.setVec3("viewPos", camera.Position);

        hudShader.use();
        hudShader.setMat4("projection", projection);

        glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(windowData.resolutionX), 0.0f, static_cast<float>(windowData.resolutionY));
        textShader.use();
        textShader.setMat4("projection", projectionText);

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        skybox->renderEntity();

        //ANIMATION TEST


        /*
        auto transforms = animator.GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            animationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(.0012f, .0012f, .0012f));
        animationShader.setMat4("model", model);
        animatedModel.Draw(directionalShader);
        */
        //
        glDepthFunc(GL_LESS);



        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->arrowInput(1);
        return;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->arrowInput(-1);
        return;
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        executeMenu = true;
    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, timeManager->getDeltaTimeUnlimitedFPS());
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, timeManager->getDeltaTimeUnlimitedFPS());
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, timeManager->getDeltaTimeUnlimitedFPS());
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, timeManager->getDeltaTimeUnlimitedFPS());
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        gamma = gamma - 0.1;
        std::cout << gamma << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        gamma = gamma + 0.1;
        std::cout << gamma << std::endl;
    }
    /*
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        //std::cout << "Joystick" << std::endl;
        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
        //std::cout << "Left Stick X: " << axes[0]  <<" Y: " << axes[1]<< std::endl;
        //std::cout << "Right Stick X: " << axes[2] << " Y: " << axes[3] << std::endl;

        GLFWgamepadstate state;

        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
        {
            if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
            {
                std::cout << "3" << std::endl;
            }
            if (state.buttons[GLFW_GAMEPAD_BUTTON_B])
            {
                std::cout << "4" << std::endl;
            }
            if (state.buttons[GLFW_GAMEPAD_BUTTON_X])
            {
                std::cout << "1" << std::endl;
            }
            if (state.buttons[GLFW_GAMEPAD_BUTTON_Y])
            {
                std::cout << "2" << std::endl;
            }
        }

    }
    */

}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (click) {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);

    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        click = true;
        firstMouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        click = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (menu->getActiveButton() != 0 && menu->isCursorOnButtons())
            executeMenu = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void set_window_size_callback(GLFWwindow* window, int x, int y)
{
    auto* localWindowData = (WindowData*)glfwGetWindowUserPointer(window);
    resizeX = (float)x / 1280;
    resizeY = (float)y / 720;
    localWindowData->resolutionX = x;
    localWindowData->resolutionY = y;
    glViewport(0, 0, x, y);
    windowSizeChanged = true;
}