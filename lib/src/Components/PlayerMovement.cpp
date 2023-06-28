#define _USE_MATH_DEFINES

#include "PlayerMovement.h"
#include "Transform.h"
#include "TimeManager.h"
#include "DynamicColliderComponent.h"
#include "StaticColliderComponent.h"
#include "TileState.h"
#include "Entity.h"
#include "PickUp.h"
#include "Tool.h"
#include "Player.h"
#include "RobotMovement.h"
#include "Audio.h"
#include "World.h"
#include "Map.h"
#include "Animator.h"
#include <cmath>
#include <math.h>

PlayerMovement::PlayerMovement(GLFWwindow* window, Entity* parent, Entity* rivalParent, Entity* robot, Transform* transform, DynamicColliderComponent* collider, DynamicColliderComponent* frontCollider, float speed, EPlayerID ID, glm::vec3 forward)
    : Component(parent)
    , window(window)
    , rivalParent(rivalParent)
    , robot(robot)
    , transform(transform)
    , collider(collider)
    , frontCollider(frontCollider)
    , speed(speed)
    , ID(ID)
    , forward(forward)
    , tool(nullptr)
{
    //std::vector<Player*> player;
    //parent.getComponentsByType(&player);
    timeManager = TimeManager::getInstance();
    timeManager->attach120FPS(this);

    audio = new Audio(parent);
    parent->addComponent(audio);

    setForward = forward;
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        //std::cout << "Joystick" << std::endl;
        const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
    }
    
    if (glfwJoystickPresent(GLFW_JOYSTICK_2)) {
        //std::cout << "Joystick" << std::endl;
        const float* axes2 = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &axisCount2);
    }
    
}

void PlayerMovement::move()
{
    std::vector<PickUp*> compTypePU;
    std::vector<Tool*> compTypeTool;
    if (ID == Player1) {
            for (auto comp : collider->getTouchingDynamicComponents()) {
                if (comp != nullptr) {
                    comp->getParent()->getComponentsByType(&compTypePU);
                    for (PickUp* pickUp : compTypePU) {
                        if(!pickUp->isUsed)
                            pickUp->use(parent);
                    }
                    comp->getParent()->getComponentsByType(&compTypeTool);
                    for (Tool* tool : compTypeTool) {
                        if (!tool->isPickedUp() && !tool->isRestarting()) {
                            if (this->tool == nullptr) {
                                this->tool = tool;
                                tool->PickedUp(Player1);
                                if (tool->getType() == EToolType::Hoe) {
                                    currentPlantTime = 0;
                                    currentDestroyTime = defaultDestroyTime;
                                }
                                if (tool->getType() == EToolType::Shovel) {
                                    currentPlantTime = defaultPlantTime;
                                    currentDestroyTime = 0;
                                }
                            }
                            else {
                                this->tool->PickedUp(None, parent->transform);
                                this->tool = tool;
                                tool->PickedUp(Player1);
                                if (tool->getType() == EToolType::Hoe) {
                                    currentPlantTime = 0;
                                    currentDestroyTime = defaultDestroyTime;
                                }
                                if (tool->getType() == EToolType::Shovel) {
                                    currentPlantTime = defaultPlantTime;
                                    currentDestroyTime = 0;
                                }

                            }
                        }
                    }
                }
            }


            previousForward = forward;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS
                || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                    //std::cout << "W" << std::endl;
                    forward.x = 1;
                    //transform->addToLocalPosition(forward * (speed*timeManager->getDeltaTime120FPS()));
                }
                else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                    //std::cout << "S" << std::endl;
                    forward.x = -1;
                    //transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                }
                else {
                    forward.x = 0;
                }
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                    //std::cout << "A" << std::endl;
                    forward.z = -1;
                    //transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                }
                else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                    //std::cout << "D" << std::endl;
                    forward.z = 1;
                    //transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                }
                else {
                    forward.z = 0;
                }
                
                forward = glm::vec3{ forward.x,0,forward.z };
                forward = glm::normalize(forward);
                transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                if (previousForward != forward) {
                    //std::cout << "inny" << std::endl;
                    float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                    if (angle == 0 || angle == -0) { angle = 180; }
                    if (setForward == forward) { angle = 0; }
                    if (angle < 0 && angle > -180) { angle = -180 - angle; }
                    //std::cout << -angle << std::endl;
                    transform->setLocalRotation({ 0,-angle,0 });
                }

                //std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
            }

            if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
                //std::cout << "Joystick" << std::endl;
                axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
                //std::cout << "Left Stick X: " << axes[0] << " Y: " << axes[1] << std::endl;
                //std::cout << "Right Stick X: " << axes[2] << " Y: " << axes[3] << std::endl;

                if (abs(axes[0]) > 0 && abs(axes[0]) < 0.1 && abs(axes[1]) > 0 && abs(axes[1]) < 0.1) {
                    forward = glm::vec3{ 0,0,0 };
                }else {
                    forward.x = -axes[1];
                    forward.z = axes[0];

                    forward = glm::vec3{ forward.x,0,forward.z };
                    forward = glm::normalize(forward);
                    transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                    //std::cout << forward.x << " " << forward.z << std::endl;
                    if (previousForward != forward) {
                        //w miare plynny obrot
                        float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                        if (angle < 0 && angle > -180) { angle = -180 - angle; }
                        if (forward.x == 1 && (forward.z < 0.1 && forward.z > -0.1)) { angle = 0; }
                        if (forward.x == -1 && (forward.z < 0.1 && forward.z > -0.1)) { angle = 180; }
                        
                        transform->setLocalRotation({ 0,-angle,0 });
                    }

                }
                GLFWgamepadstate state;

                if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
                {
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                    {
                        //std::cout << "DOWN" << std::endl;
                    }
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_B])
                    {
                        //std::cout << "RIGHT" << std::endl;
                    }
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_X])
                    {
                        //std::cout << "LEFT" << std::endl;
                    }
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_Y])
                    {
                        //std::cout << "UP" << std::endl;
                    }
                }

            }
    }
    if (ID == Player2) {
        for (auto comp : collider->getTouchingDynamicComponents()) {
            if (comp != nullptr) {
                comp->getParent()->getComponentsByType(&compTypePU);
                for (PickUp* pickUp : compTypePU) {
                    if (!pickUp->isUsed)
                        pickUp->use(parent);
                }
                comp->getParent()->getComponentsByType(&compTypeTool);
                for (Tool* tool : compTypeTool) {
                    if (!tool->isPickedUp() && !tool->isRestarting()) {
                        if (this->tool == nullptr) {
                            this->tool = tool;
                            tool->PickedUp(Player2);
                            if (tool->getType() == EToolType::Hoe) {
                                currentPlantTime = 0;
                                currentDestroyTime = defaultDestroyTime;
                            }
                            if (tool->getType() == EToolType::Shovel) {
                                currentPlantTime = defaultPlantTime;
                                currentDestroyTime = 0;
                            }
                        }
                        else {
                            this->tool->PickedUp(None, parent->transform);
                            this->tool = tool;
                            tool->PickedUp(Player2);
                            if (tool->getType() == EToolType::Hoe) {
                                currentPlantTime = 0;
                                currentDestroyTime = defaultDestroyTime;
                            }
                            if (tool->getType() == EToolType::Shovel) {
                                currentPlantTime = defaultPlantTime;
                                currentDestroyTime = 0;
                            }

                        }
                    }
                }
            }
        }

        previousForward = forward;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
            || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                forward.x = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                forward.x = -1;
            }
            else {
                forward.x = 0;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                forward.z = -1;
            }
            else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                forward.z = 1;
            }
            else {
                forward.z = 0;
            }

            forward = glm::vec3{ forward.x,0,forward.z };
            forward = glm::normalize(forward);
            transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
            if (previousForward != forward) {
                float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                if (angle == 0 || angle == -0) { angle = 180; }
                if (setForward == forward) { angle = 0; }
                if (angle < 0 && angle > -180) { angle = -180 - angle; }
                transform->setLocalRotation({ 0,-angle,0 });
            }

        }
        
        if (glfwJoystickPresent(GLFW_JOYSTICK_2)) {
            //std::cout << "Joystick" << std::endl;
            axes2 = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &axisCount2);
            //std::cout << "Left Stick X: " << axes2[0] << " Y: " << axes2[1] << std::endl;
            //std::cout << "Right Stick X: " << axes2[2] << " Y: " << axes2[3] << std::endl;

            if (abs(axes2[0]) > 0 && abs(axes2[0]) < 0.1 && abs(axes2[1]) > 0 && abs(axes2[1]) < 0.1) {
                forward = glm::vec3{ 0,0,0 };
                //std::cout << forward.x << " " << forward.z << std::endl;
            }
            else {
                forward.x = -axes2[1];
                forward.z = axes2[0];

                forward = glm::vec3{ forward.x,0,forward.z };
                forward = glm::normalize(forward);
                transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                //std::cout << forward.x << " " << forward.z << std::endl;
                if (previousForward != forward) {
                    float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                    if (angle < 0 && angle > -180) { angle = -180 - angle; }
                    if (forward.x == 1 && (forward.z < 0.1 && forward.z > -0.1)) { angle = 0; }
                    if (forward.x == -1 && (forward.z < 0.1 && forward.z > -0.1)) { angle = 180; }

                    transform->setLocalRotation({ 0,-angle,0 });
                }

            }
            GLFWgamepadstate state;

            if (glfwGetGamepadState(GLFW_JOYSTICK_2, &state))
            {
                if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                {
                    //std::cout << "DOWN" << std::endl;
                }
                if (state.buttons[GLFW_GAMEPAD_BUTTON_B])
                {
                    //std::cout << "RIGHT" << std::endl;
                }
                if (state.buttons[GLFW_GAMEPAD_BUTTON_X])
                {
                    //std::cout << "LEFT" << std::endl;
                }
                if (state.buttons[GLFW_GAMEPAD_BUTTON_Y])
                {
                    //std::cout << "UP" << std::endl;
                }
            }

        }
        
    }
}

void PlayerMovement::update() {
    move();
    checkInput();
    handleSeenTile();
    handleActionTimer();
    if(rivalPunched && rivalPunchTimer < rivalPunchTime) {
        updateRivalAfterPunch(rivalPunchedDistance);
    }
    else if (rivalPunchTimer >= rivalPunchTime) {
        rivalPunched = false;
        std::vector<PlayerMovement*> playerMovement;
        rivalParent->getComponentsByType(&playerMovement);
        playerMovement[0]->setSpeed(1.0f);
    }
    if (robotPunched && robotPunchTimer < robotPunchTime) {
        updateRobotAfterPunch(robotPunchedDistance);
    }
    else if (robotPunchTimer >= robotPunchTime) {
        robotPunched = false;
    }
    if (cooldownTimer > 0) {
        cooldownTimer -= timeManager->getDeltaTime120FPS();
    }
}

void PlayerMovement::enable(bool value) {
    if (enabled) {
        TimeManager::getInstance()->detach(this);
        enabled = false;
    }
    if (value) {
        TimeManager::getInstance()->attach120FPS(this);
        enabled = true;
    }
}

void PlayerMovement::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

float PlayerMovement::getSpeed()
{
    return speed;
}

void PlayerMovement::dropTool()
{
    if (tool) {
        tool->PickedUp(None, parent->transform);
        tool = nullptr;
    }
}

void PlayerMovement::checkInput(){
    StaticColliderComponent *tileCollider = frontCollider->getTileColliderIAmOn();
    TileState *tileState = nullptr;
    if (tileCollider != nullptr) tileState = tileCollider->getTileState();
    switch(ID){
        case Player1:
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
                startAction(tileState);
            }
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                for (int i = 0; i < frontCollider->getTouchingDynamicComponents().size(); i++) {
                    if(cooldownTimer <= 0.f ){
                        startAction(nullptr, false);
                        //std::cout << "parent 1 " << parent << std::endl;
                        //std::cout << "at " << frontCollider->getTouchingDynamicComponents().at(i)->getParent() << std::endl;
                        if (rivalParent == frontCollider->getTouchingDynamicComponents().at(i)->getParent()) {
                            //std::cout << "punch" << std::endl;
                            reactToPunch(rivalParent);
                            break;
                        }
                        if (robot == frontCollider->getTouchingDynamicComponents().at(i)->getParent()) {
                            //std::cout << "punch robot" << std::endl;
                            reactToPunchRobot();
                            break;
                        }
                    }
                }
            }
            if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
                GLFWgamepadstate state;
                if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
                {
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                    {

                        startAction(tileState);
                    }

                    if (state.buttons[GLFW_GAMEPAD_BUTTON_X])
                    {
                        for (int i = 0; i < frontCollider->getTouchingDynamicComponents().size(); i++) {

                            if (cooldownTimer <= 0.f) {
                                startAction(nullptr, false);
                                //std::cout << "parent 1 " << parent << std::endl;
                                //std::cout << "at " << frontCollider->getTouchingDynamicComponents().at(i)->getParent() << std::endl;
                                if (rivalParent == frontCollider->getTouchingDynamicComponents().at(i)->getParent()) {
                                    //std::cout << "punch" << std::endl;
                                    reactToPunch(rivalParent);
                                    break;
                                }
                                if (robot == frontCollider->getTouchingDynamicComponents().at(i)->getParent()) {
                                    //std::cout << "punch robot" << std::endl;
                                    reactToPunchRobot();
                                    break;
                                }
                            }
                        }

                    }
                }
            }

            break;
        case Player2:
            if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
                startAction(tileState);
            }
            if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
                for (int i = 0; i < frontCollider->getTouchingDynamicComponents().size(); i++) {
                    if (cooldownTimer <= 0.f) {
                        startAction(nullptr, false);
                        if (rivalParent == frontCollider->getTouchingDynamicComponents().at(i)->getParent()) {
                            //std::cout << "punch" << std::endl;
                            reactToPunch(rivalParent);
                            break;
                        }
                        if (robot == frontCollider->getTouchingDynamicComponents().at(i)->getParent()) {
                            //std::cout << "punch robot" << std::endl;
                            reactToPunchRobot();
                            break;
                        }
                    }
                }
            }
            if (glfwJoystickPresent(GLFW_JOYSTICK_2)) {
                GLFWgamepadstate state;
                if (glfwGetGamepadState(GLFW_JOYSTICK_2, &state))
                {
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                    {
                        startAction(tileState);
                    }

                    if (state.buttons[GLFW_GAMEPAD_BUTTON_X])
                    {
                        for (int i = 0; i < frontCollider->getTouchingDynamicComponents().size(); i++) {
                            if (cooldownTimer <= 0.f) {
                                startAction(nullptr, false);
                                
                                if (rivalParent == frontCollider->getTouchingDynamicComponents().at(i)->getParent()) {
                                    //std::cout << "punch" << std::endl;
                                    reactToPunch(rivalParent);
                                    break;
                                }
                                if (robot == frontCollider->getTouchingDynamicComponents().at(i)->getParent()) {
                                    //std::cout << "punch robot" << std::endl;
                                    reactToPunchRobot();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
}

void PlayerMovement::handleSeenTile(){
    if (!isTimerSet)
    {
        if (lastSeenTile == nullptr) {
            lastSeenTile = frontCollider->getTileColliderIAmOn();
            if (lastSeenTile == nullptr)
                return;

            lastSeenTile->getParent()->switchShader();
        }
        else if (lastSeenTile != frontCollider->getTileColliderIAmOn()) {
            lastSeenTile->getParent()->switchShader();
            lastSeenTile = frontCollider->getTileColliderIAmOn();
            if (lastSeenTile != nullptr)
                lastSeenTile->getParent()->switchShader();
        }
    }
}

void PlayerMovement::handleActionTimer()
{
    actionTimer -= timeManager->getDeltaTime120FPS();
    if (isTimerSet && actionTimer <= 0)
    {
        actionTile->changeTileState(ID, currentAction);
        cancelAction(actionTile);

    }
}

void PlayerMovement::resetSeenTile(){
    if (lastSeenTile)
        lastSeenTile->getParent()->switchShader();
}



void PlayerMovement::startAction(TileState* tile, bool isHit)
{
    if (currentAction == EActionType::Idle || currentAction == EActionType::Moving)
    {
        EActionType action;
        if (tile != nullptr)
        {
            EState state = tile->state;
            switch (state)
            {
            case Empty:
                if (rival->actionTile == tile) return;
                {
                    World* world = World::getInstance();
                    Map* map = world->mapComponent;
                    if (map->getSeedCount(ID) == 0)
                        return;
                    animator->PlayAnimation(PlayerAnimType::TileAction);
                    audio->playMusic("res/audio/planting.wav");
                    action = Planting;
                }
                break;
            case Growing:
                if (ID == Player1)
                {
                    if (rival->actionTile == tile) return;
                    if (tool != nullptr && tool->getType() == WateringCan)
                    {
                        animator->PlayAnimation(PlayerAnimType::TileAction);
                        action = Watering;
                    }
                    else return;
                }
                else
                {
                    rival->cancelAction(tile);
                    animator->PlayAnimation(PlayerAnimType::TileAction);
                    action = DestroyingFlower;
                }
                break;
            case Growing2:
                if (ID == Player2)
                {
                    if (rival->actionTile == tile) return;
                    if (tool != nullptr && tool->getType() == WateringCan)
                    {
                        animator->PlayAnimation(PlayerAnimType::TileAction);
                        action = Watering;
                    }
                    else return;
                }
                else
                {
                    rival->cancelAction(tile);
                    animator->PlayAnimation(PlayerAnimType::TileAction);
                    action = DestroyingFlower;
                }
                break;
            case Grown:
                if (ID == Player1)
                {
                    if (rival->actionTile == tile) return;
                    animator->PlayAnimation(PlayerAnimType::TileAction);
                    action = Harvesting;
                }
                else {
                    rival->cancelAction(tile);
                    animator->PlayAnimation(PlayerAnimType::TileAction);
                    action = DestroyingFlower;
                }
                break;
            case Grown2:
                if (ID == Player2)
                {
                    if (rival->actionTile == tile) return;
                    animator->PlayAnimation(PlayerAnimType::TileAction);
                    action = Harvesting;
                }
                else {
                    rival->cancelAction(tile);
                    animator->PlayAnimation(PlayerAnimType::TileAction);
                    action = DestroyingFlower;
                }
                break;
            case Overgrown:
                if (rival->actionTile == tile) return;
                animator->PlayAnimation(PlayerAnimType::TileAction);
                action = DestroyingOvergrown;
                break;
            case Burned:
                return;
            }
            currentAction = action;
            actionTile = tile;
        }
        else
        {
            if (isHit)
            {
                animator->PlayAnimation(PlayerAnimType::Punched);
                action = BeingHit;
            }
            else {
                animator->PlayAnimation(PlayerAnimType::Punching);
                action = Hitting;
            }
            
        }
        

        switch (action)
        {
        case Planting:
            actionTimer = currentPlantTime;
            isTimerSet = true;
            break;
        case DestroyingFlower:
        case DestroyingOvergrown:
            actionTimer = currentDestroyTime;
            isTimerSet = true;
            audio->playMusic("res/audio/fire.wav");
            break;
        case Harvesting:
            actionTimer = currentHarvestTime;
            isTimerSet = true;
            break;
        case Watering:
            actionTimer = currentWaterTime;
            isTimerSet = true;
            break;
        case Hitting:
            actionTimer = currentHitTime;
            isTimerSet = true;
            break;
        case BeingHit:
            actionTimer = currentBeingHitTime;
            isTimerSet = true;
        }
    }
}

void PlayerMovement::cancelAction(TileState* tile)
{
    animator->PlayAnimation(PlayerAnimType::Standing);
    if (tile == nullptr) tile = actionTile;
    if (tile == actionTile)
    {
        currentAction = EActionType::Idle;
        actionTimer = 0;
        isTimerSet = false;
        actionTile = nullptr;
    }
}

void PlayerMovement::setRivalPlayerMovement(PlayerMovement* rivalPlayerMovement)
{
    this->rival = rivalPlayerMovement;
}

void PlayerMovement::setAnimator(std::shared_ptr<Animator> animator)
{
    this->animator = animator;
}

void PlayerMovement::reactToPunch(Entity* punchedParent)
{
    cooldownTimer = cooldownForPunch;
    audio->playMusic("res/audio/punch.wav");
    rivalPunched = true;
    rivalPunchTimer = 0.0f;

    std::vector<PlayerMovement*> playerMovement;
    punchedParent->getComponentsByType(&playerMovement);
    playerMovement[0]->dropTool();

    //float prevSpeed = playerMovement[0]->getSpeed();
    playerMovement[0]->setSpeed(0);
    playerMovement[0]->startAction(nullptr, true);
    glm::vec3 rivalPosition = rivalParent->transform->getGlobalPosition();
    glm::vec3 myPosition = parent->transform->getGlobalPosition();
    glm::vec3 difference = rivalPosition - myPosition;
    difference = glm::normalize(difference);
    glm::vec3 actualDifference = glm::vec3{ 5,0,5 } *difference;
    //std::cout << actualDifference.x << " " << actualDifference.z << std::endl;

    actualDifference.x = actualDifference.x / (rivalPunchTime / (timeManager->getDeltaTime120FPS()));
    actualDifference.z = actualDifference.z / (rivalPunchTime / (timeManager->getDeltaTime120FPS()));

    rivalPunchedDistance = actualDifference;

}

void PlayerMovement::updateRivalAfterPunch(glm::vec3 distance) {

    rivalParent->transform->addToLocalPosition(distance * timeManager->getDeltaTime120FPS());
    rivalPunchTimer += timeManager->getDeltaTime120FPS();
}

void PlayerMovement::updateRobotAfterPunch(glm::vec3 distance) {

    robot->transform->addToLocalPosition(distance * timeManager->getDeltaTime120FPS());
    robotPunchTimer += timeManager->getDeltaTime120FPS();
}

void PlayerMovement::reactToPunchRobot()
{
    cooldownTimer = cooldownForPunch;
    audio->playMusic("res/audio/punch.wav");
    robotPunched = true;
    robotPunchTimer = 0.0f;

    std::vector<RobotMovement*> robotMovement;
    robot->getComponentsByType(&robotMovement);
    glm::vec3 rivalPosition = robot->transform->getGlobalPosition();
    glm::vec3 myPosition = parent->transform->getGlobalPosition();
    glm::vec3 difference = rivalPosition - myPosition;
    difference = glm::normalize(difference);
    glm::vec3 actualDifference = glm::vec3{ 5,0,5 } * difference;

    actualDifference.x = actualDifference.x / (robotPunchTime / (timeManager->getDeltaTime120FPS()));
    actualDifference.z = actualDifference.z / (robotPunchTime / (timeManager->getDeltaTime120FPS()));

    robotPunchedDistance = actualDifference;
}
