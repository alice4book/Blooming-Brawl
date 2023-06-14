#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Audio.h"
#include "Shader.h"
#include "Camera.h"
#include "World.h"
#include "RobotMovement.h"
#include "StaticColliderComponent.h"
#include "DynamicColliderComponent.h"
#include "PickUp.h"
#include "Spawner.h"

#include "Player.h"
#include "PlayerMovement.h"

#include "Model.h"
#include "Map.h"
#include "HUD.h"
#include "Tool.h"

#include "TimeManager.h"
#include "Menu.h"
#include "Round.h"

#define GLFW_GAMEPAD_BUTTON_A 0
#define GLFW_GAMEPAD_BUTTON_B 1
#define GLFW_GAMEPAD_BUTTON_X 2
#define GLFW_GAMEPAD_BUTTON_Y 3

#define TILE_SIZE 0.254f

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void set_window_size_callback(GLFWwindow* window, int x, int y);

// camera
Camera camera(glm::vec3(0.0f, 3.f, 0.0f), glm::vec3(0,1,0), 0, -75.0f);
float lastX;
float lastY;
bool firstMouse = true;

// timing
TimeManager* timeManager = TimeManager::getInstance();
bool click = false;

//gamepad
int axisCount;

struct WindowData {
    int resolutionX, resolutionY;
} windowData{1280, 720};

float resizeX = 1.f;
float resizeY = 1.f;


float gamma = 1;

int ChosenButtonMenu = 1;

int roundNr = 1;

int main()
{
    // Setup window
    if (!glfwInit())
        return 1;

    std::srand(time(NULL));

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

    glfwSetWindowUserPointer(window, (void* ) &windowData);
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

    glm::vec3 dirLightColor(1.0f, 1.0f, 1.0f);

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
    //round.getMapManager()->addChild(&hud);
    //round.getMap()->addHud(&hud);

    Round round(window, tileModels, mapFiles, &directionalShader, &pickupShader, &highlightShader, &hud);

    camera.setCameraPosition(TILE_SIZE, 4.7f, round.getMap()->MAX_COLUMNS, round.getMap()->MAX_ROWS);
#pragma endregion

#pragma region House
    Entity background("res/models/background_trawa.obj", &directionalShader);
    Entity house1("res/models/dom.obj", &directionalShader);
    Entity house2("res/models/dom.obj", &directionalShader);
    background.transform->setLocalPosition({2.f,-0.5f,2.f});
    house2.transform->setLocalRotation({0.f,180.f,0.f});
    house2.transform->setLocalPosition({ 2.5f,0.f, 4.8f});
    skybox->addChild(&background);
    skybox->addChild(&house1);
    skybox->addChild(&house2);
#pragma endregion

#pragma region Power Up Setting
    float rimLight = 0.5f;
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
    glm::mat4 lightView = glm::lookAt(glm::vec3(-1.0f, 2.0f, -1.0f), //eye
                                    glm::vec3(0.0f, 0.0f, 0.0f), //center
                                       glm::vec3(0.0f, 1.0f, 0.0f)); //up (y)
    glm::mat4 lightProjection = orthogonalProjection * lightView;

    //glEnable(GL_FRAMEBUFFER_SRGB);

#pragma endregion

#pragma region Menu
    Menu menu(&hudShader, &textShader);
    skybox->addChild(&menu);
#pragma endregion

#pragma region Audio   
    Audio audioBackground(round.getRobot());
    audioBackground.playMusic("res/audio/x.wav", true);
#pragma endregion
    int currentRound = 0;
    // render loop
    while (!glfwWindowShouldClose(window))
    {

        if (currentRound < 4 && currentRound != hud.currentMap()) {
            round.changeRound(hud.currentMap());
            currentRound = hud.currentMap();
        }
        // input
        processInput(window);

        switch(ChosenButtonMenu)
        {
            case 0:
                menu.setActiveButton(0);
                hud.setHideHud(false);
                hud.setResize(resizeX, resizeY);
                timeManager->updateTime();
                camera.setCameraRotation(0,-75.f);
                break;
            case 1:
                menu.setActiveButton(1);
                hud.setHideHud(true);
                camera.setCameraRotation(0,0);
                break;
            case 2:
                menu.setActiveButton(2);
                hud.setHideHud(true);
                break;
            default:
                // WTF how?
                break;
        }

        glfwPollEvents();

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
        round.getRobot()->renderEntity(&depthShader);
        round.getPlayer1()->renderEntity(&depthShader);
        round.getPlayer2()->renderEntity(&depthShader);
        round.getMapManager()->renderEntity(&depthShader);
        for (int i = 0; i < round.getToolsSize(); i++) {
            round.getTools()[i]->renderEntity(&depthShader);
        }
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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) windowData.resolutionX / (float) windowData.resolutionY, 0.1f, 100.0f);
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
        highlightShader.setVec3("hlcolor", {0.5,0.5,0.5});

        directionalShader.use();
        glUniform1i(glGetUniformLocation(depthShader.ID, "depthMap"), 1);
        directionalShader.setMat4("lightProjection",lightProjection);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        directionalShader.setInt("texture_diffuse1", 0);
        glUniform1i(glGetUniformLocation(directionalShader.ID, "depthMap"), 2);
        directionalShader.setFloat("gamma", gamma);
        directionalShader.setMat4("projection", projection);
        directionalShader.setMat4("view", view);
        directionalShader.setVec4("aColor", glm::vec4(dirLightColor, 0.0f));
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
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
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
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && roundNr != 3) {
        roundNr = 2;
    }

    if(ChosenButtonMenu == 0)
        return;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        ChosenButtonMenu = 1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        ChosenButtonMenu = 2;
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
        switch(ChosenButtonMenu){
            case 1:
                // start the game
                ChosenButtonMenu = 0;
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
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
//        std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
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
}