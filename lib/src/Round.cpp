#include "Round.h"
#include "World.h"
#include "DynamicColliderComponent.h"
#include "PathFinding.h"
#include "RobotMovement.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "Map.h"
#include "Model.h"
#include <iostream>

Round::Round(GLFWwindow* window, Model* tileModels, std::string* mapFiles, Shader* directionalShader,
    Shader* pickupShader, Shader* highlightShader)
{
	skybox = World::getInstance();
    
    mapManager = new Entity(directionalShader);
    map = new Map(mapManager, tileModels, mapFiles, TILE_SIZE, pickupShader, highlightShader, 0);
    mapManager->addComponent((Component*)map);
    skybox->addChild(mapManager);

    robot = new Entity("res/models/robot.obj", directionalShader);
    skybox->addChild(robot);
    robot->transform->rotateLocal(glm::vec3(0.0f, 90.0f, 0.0f));
    
    DynamicColliderComponent* robotCollider = new DynamicColliderComponent(robot, 0.1f, false, { 0,0 });
    robot->addComponent((Component*)robotCollider);
    DynamicColliderComponent* robotColliderFront = new DynamicColliderComponent(robot, 0.1f, true, { 0.01f, 0.0f });
    robot->addComponent((Component*)robotColliderFront);
    if (map == nullptr) {
        std::cout << "nullptr" << std::endl;
    }
    PathFinding pathFinding((Map*)map);
    
    RobotMovement* robotmovement = new RobotMovement(robot, robot->transform, robotCollider,robotColliderFront, 0.2f, 
        pathFinding, this->TILE_SIZE, { 0,0,1 });
    robot->addComponent((Component*)robotmovement);
    robotmovement->findClosestNode();
    
    player1 = new Entity ("res/models/postacie_zeskalowne/nizej_farmer.obj", directionalShader);
    skybox->addChild(player1);
    player2 = new Entity("res/models/postacie_zeskalowne/nizej_farmer.obj", directionalShader);
    skybox->addChild(player2);
    
    Player* playerP1 =  new Player(player1, Player1);
    player1->addComponent((Component*)playerP1);
    DynamicColliderComponent* player1Collider = new DynamicColliderComponent(player1, 0.05f, false);
    player1->addComponent((Component*)player1Collider);
    DynamicColliderComponent* player1ColliderFront = new DynamicColliderComponent(player1, 0.2f, true, { 0.15f,0 });
    player1->addComponent((Component*)player1Collider);
    PlayerMovement* playerMovement = new PlayerMovement(window, player1, player2, robot, player1->transform, player1Collider,
        player1ColliderFront, playerP1->getSpeed(), playerP1->getID(), { 1,0,0 });
    player1->addComponent((Component*)playerMovement);

    Player* playerP2 = new Player(player2, Player2);
    player2->addComponent((Component*)playerP2);
    DynamicColliderComponent* player2Collider = new DynamicColliderComponent(player2, 0.05f, false);
    player2->addComponent((Component*)player2Collider);
    DynamicColliderComponent* player2ColliderFront = new DynamicColliderComponent(player2, 0.2f, true, { 0.15f,0 });
    player2->addComponent((Component*)player2ColliderFront);
    PlayerMovement* playerMovement2 = new PlayerMovement(window, player2, player1, robot, player2->transform, player2Collider,
        player2ColliderFront, playerP2->getSpeed(), playerP2->getID(), { 1,0,0 });
    player2->addComponent((Component*)playerMovement2);
    
    player2->transform->setLocalPosition({ 0.6, 0, 2.4 });
    player1->transform->setLocalPosition({ 0.5, 0, 2.4 });
    robot->transform->setLocalPosition({ 0.4, 0, 2.4 });
}

Entity* Round::getPlayer1()
{
    return player1;
}

Entity* Round::getPlayer2()
{
    return player2;
}

Entity* Round::getRobot()
{
    return robot;
}

Entity* Round::getMapManager()
{
    return mapManager;
}

Map* Round::getMap()
{
    return map;
}

