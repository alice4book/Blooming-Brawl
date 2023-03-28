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
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
//#include "Entity.h"
#include "RobotMovement.h"
#include "StaticColliderComponent.h"
#include "DynamicColliderComponent.h"

#include "Player.h"
#include "PlayerMovement.h"

#define GLFW_GAMEPAD_BUTTON_A 0
#define GLFW_GAMEPAD_BUTTON_B 1
#define GLFW_GAMEPAD_BUTTON_X 2
#define GLFW_GAMEPAD_BUTTON_Y 3

#define TILE_SIZE 0.254f

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// camera
Camera camera(glm::vec3(0.0f, 3.f, 0.0f), glm::vec3(0,1,0), 0, -89.0f);
float lastX;
float lastY;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
bool click = false;

//gamepad
int axisCount;

struct Direction {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 color;
};

int main()
{
    // Setup window
    if (!glfwInit())
        return 1;

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

//    Borderless windowed mode
//
//    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
//    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
//    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
//    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
//
//    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Farm Engine", monitor, NULL);

    //Windowed mode
    GLFWwindow* window = glfwCreateWindow(mode->width / 2, mode->height / 2, "Farm Engine", NULL, NULL);

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

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
#pragma endregion

    // build and compile our shader zprogram
    Shader modelShader("res/shaders/vertexModel.vert", "res/shaders/fragment.frag");
    Shader skyboxShader("res/shaders/vertexSkybox.vert", "res/shaders/fragmentSkybox.frag");

#pragma region skybox
    // set up vertex data (and buffer(s)) and configure vertex attributes
    std::vector <float> cube;
    cube.insert(cube.end(), {
            -1.0, 1.0, 1.0,    -1, 1, 1,    0.0, 1.0, //0
            1.0, 1.0, 1.0,     1, 1, 1,     1.0, 1.0, //1
            1.0, -1.0, 1.0,    1, -1, 1,    1.0, 0.0, //2
            -1.0, -1.0, 1.0,   -1, -1, 1,   0.0, 0.0, //3

            -1.0, 1.0, -1.0,   -1, 1, -1,   1.0, 1.0, //4
            1.0, 1.0, -1.0,    1, 1, -1,    0.0, 1.0, //5
            1.0, -1.0, -1.0,   1, -1, -1,   0.0, 0.0, //6
            -1.0, -1.0, -1.0,  -1, -1, -1,  1.0, 0.0  //7
    });

    std::vector <int> indicesCube;
    indicesCube.insert(indicesCube.end(), {
            0, 1, 2,
            0, 3, 2,

            1, 2, 6,
            1, 5, 6,

            0, 1, 5,
            0, 4, 5,

            0, 4, 7,
            0, 3, 7,

            4, 5, 6,
            4, 7, 6,

            7, 6, 2,
            7, 3, 2
    });
    std::vector<std::string> faces
            {
                    "res/textures/skybox/right.jpg",
                    "res/textures/skybox/left.jpg",
                    "res/textures/skybox/top.jpg",
                    "res/textures/skybox/bottom.jpg",
                    "res/textures/skybox/front.jpg",
                    "res/textures/skybox/back.jpg"
            };
#pragma endregion

    Entity skybox = Entity(cube, indicesCube, &skyboxShader);
    skybox.loadCubemap(faces);

    Entity grass("res/models/trawa.obj", &modelShader);
    Entity big_flower("res/models/duzy_kwiat.obj", &modelShader);
    Entity small_flower("res/models/maly_kwiat.obj", &modelShader);
    Entity burnt_flower("res/models/spalony.obj", &modelShader);
    Entity rock1("res/models/skaly.obj", &modelShader);
    Entity rock2("res/models/skaly.obj", &modelShader);
    Entity rock3("res/models/skaly.obj", &modelShader);
    Entity rock4("res/models/skaly.obj", &modelShader);
    Entity rock5("res/models/skaly.obj", &modelShader);

    big_flower.transform->setLocalPosition({ TILE_SIZE, 0, 0 });
    grass.transform->setLocalPosition({ TILE_SIZE * 2, 0, 0 });
    small_flower.transform->setLocalPosition({ TILE_SIZE * 3, 0, 0 });
    burnt_flower.transform->setLocalPosition({ TILE_SIZE * 4, 0, 0 });

    rock1.transform->setLocalPosition({ 0, 0, 0 });
    rock2.transform->setLocalPosition({ TILE_SIZE, 0, TILE_SIZE });
    rock3.transform->setLocalPosition({ 0, 0, 2 * TILE_SIZE });
    rock4.transform->setLocalPosition({ -4 * TILE_SIZE, 0, TILE_SIZE });
    rock5.transform->setLocalPosition({ -3 * TILE_SIZE, 0, 0 });

    skybox.addChild(&grass);
    skybox.addChild(&big_flower);
    skybox.addChild(&small_flower);
    skybox.addChild(&burnt_flower);
    
    skybox.addChild(&rock1);
    skybox.addChild(&rock2);
    skybox.addChild(&rock3);
    skybox.addChild(&rock4);
    skybox.addChild(&rock5);

#pragma region Collision & Robot test
    StaticColliderComponent rockCollider1(&rock1, {TILE_SIZE,TILE_SIZE}, false);
    rock1.addComponent((Component*)&rockCollider1);
    StaticColliderComponent rockCollider2(&rock2, {TILE_SIZE,TILE_SIZE}, false);
    rock2.addComponent((Component*)&rockCollider2);
    StaticColliderComponent rockCollider3(&rock3, {TILE_SIZE,TILE_SIZE}, false);
    rock3.addComponent((Component*)&rockCollider3);
    StaticColliderComponent rockCollider4(&rock4, {TILE_SIZE,TILE_SIZE}, false);
    rock4.addComponent((Component*)&rockCollider4);
    StaticColliderComponent rockCollider5(&rock5, {TILE_SIZE,TILE_SIZE}, false);
    rock5.addComponent((Component*)&rockCollider5);

    //add and move robot1 (version robot turns only right)
    Entity robot1("res/models/robot.obj", &modelShader);
    skybox.addChild(&robot1);
    robot1.transform->setLocalPosition({0, 0, 0.25});
    DynamicColliderComponent robotCollider1(&robot1, 0.1f);
    robot1.addComponent((Component*)&robotCollider1);
    RobotMovement robotmovement(&robot1, robot1.transform, &robotCollider1,0.02f, {0,0,-1});
    robot1.addComponent((Component*)&robotmovement);
    ////add and move robot2
    //Entity robot2("res/models/robot.obj", &modelShader);
    //skybox.addChild(&robot2);
    //robot2.transform->setLocalPosition({5, 0, 0});
    //robot2.addComponent(new RobotMovement(&robot2, robot2.transform, 0.001f, {-1,0,0}));
    //DynamicColliderComponent robotCollider2(&robot2, 0.1f);
    //robot2.addComponent((Component*)&robotCollider2);
    ////add and move robot3
    //Entity robot3("res/models/robot.obj", &modelShader);
    //skybox.addChild(&robot3);
    //robot3.transform->setLocalPosition({-5, 0, -5});
    //robot3.addComponent(new RobotMovement(&robot3, robot3.transform, 0.001f, {1,0,1}));
    //DynamicColliderComponent robotCollider3(&robot3, 0.1f);
    //robot3.addComponent((Component*)&robotCollider3);
    Audio audio1(&robot1);
    audio1.playAudio("res/audio/powerup.wav");
    int b = 0;


    Player player("res/models/robot.obj", &modelShader, Player1);
    skybox.addChild(&player);
    player.transform->setLocalPosition({ 0,0,0.25 });
    DynamicColliderComponent playerCollider1(&player, 0.1f);
    player.addComponent((Component*)&playerCollider1);
    PlayerMovement playerMovement(&player, player.transform, &playerCollider1, player.getSpeed(), player.getID(), {0,0,-1});
    player.addComponent((Component*)&playerMovement);

    Player player2("res/models/robot.obj", &modelShader, Player2);
    skybox.addChild(&player2);
    player2.transform->setLocalPosition({ 0,0,0.5 });
    DynamicColliderComponent playerCollider2(&player2, 0.1f);
    player2.addComponent((Component*)&playerCollider2);
    PlayerMovement playerMovement2(&player2, player2.transform, &playerCollider2, player2.getSpeed(), player2.getID(), {0,0,-1});
    player2.addComponent((Component*)&playerMovement2);

#pragma endregion


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        playerMovement.move(window);
        playerMovement2.move(window);

        glfwPollEvents();        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)mode->width / (float)mode->height, 0.1f, 100.0f);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        // activate shader
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setVec3("viewPos", camera.Position);
       
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        skybox.renderEntity();
        glDepthFunc(GL_LESS);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    */
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        std::cout << "Key pressed: R    Player1 plant" << std::endl;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        std::cout << "Key pressed: T    Player1 hit" << std::endl;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        std::cout << "Key pressed: Y    Player1 time" << std::endl;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        std::cout << "Key pressed: M    Player2 time" << std::endl;
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
        std::cout << "Key pressed: ,    Player2 hit" << std::endl;
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
        std::cout << "Key pressed: .    Player2 plant" << std::endl;

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
        std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
