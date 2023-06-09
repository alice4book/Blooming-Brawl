#include "Round.h"
#include "World.h"
#include "DynamicColliderComponent.h"
#include "PathFinding.h"
#include "RobotMovement.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "Map.h"
#include "Model.h"
#include "Tool.h"
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
    
    player1->transform->setLocalPosition(map->getPlayer2Cord());
    player2->transform->setLocalPosition(map->getPlayer1Cord());
    robot->transform->setLocalPosition(map->getRobotCord());

    allTools[0] = new Entity("res/models/lopata.obj", directionalShader);
    DynamicColliderComponent* tool1_collision = new DynamicColliderComponent(allTools[0], 0.05f, true);
    allTools[0]->addComponent((Component*)tool1_collision);
    allTools[0]->addComponent(new Tool(allTools[0]));

    allTools[1] = new Entity("res/models/motyka.obj", directionalShader);
    DynamicColliderComponent* tool2_collision = new DynamicColliderComponent(allTools[1], 0.05f, true);
    allTools[1]->addComponent((Component*)tool2_collision);
    allTools[1]->addComponent(new Tool(allTools[1]));
    
    mapManager->addChild(allTools[0]);
    mapManager->addChild(allTools[1]);
    int toolNr = 0;
    std::vector<Entity*> toolstab;
    toolstab.push_back(allTools[0]);
    toolstab.push_back(allTools[1]);
    std::vector<glm::vec3> toolscord = map->getToolsCord();
    for (int i = 0; i < toolscord.size(); i++) {
        toolNr = std::rand() % toolstab.size();
        std::vector<Tool*> vectorTool;
        toolstab[toolNr]->getComponentsByType(&vectorTool);
        vectorTool[0]->setSpawn();
        toolstab[toolNr]->transform->setLocalPosition(toolscord[i]);
        toolstab.erase(toolstab.begin() + toolNr);
    }

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

Entity** Round::getTools()
{
    return allTools;
}

int Round::getToolsSize()
{
    return 2;
}
