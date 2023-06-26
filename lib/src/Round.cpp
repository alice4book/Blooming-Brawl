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
#include "HUD.h"
#include "Clock.h"
#include <iostream>

Round::Round(GLFWwindow* window, Model* tileModels, std::string* mapFiles, Shader* directionalShader,
    Shader* pickupShader, Shader* highlightShader, HUD* hud)
{

    roundTime = 20;


	auto skybox = World::getInstance();
    
    round = new Entity();
    skybox->addChild(round);

    this->hud = hud;

    for (int i = 0; i < 5; i++) {
        mapManagers[i] = new Entity(directionalShader);
        maps[i] = new Map(mapManagers[i], tileModels, mapFiles, TILE_SIZE, pickupShader, highlightShader, i);
        mapManagers[i]->addComponent(maps[i]);
        maps[i]->GenerateMap(i);
    }

    currentMap = maps[0];
    currentMapManager = mapManagers[0];
    skybox->mapComponent = currentMap;

    this->hud->clock = new Clock(hud);
    this->hud->addComponent(hud->clock);
    this->hud->clock->startClock(roundTime);
    skybox->addChild(hud);
    currentMap->addHud(hud);
    skybox->mapComponent = currentMap;

    round->addChild(currentMapManager);

    robot = new Entity("res/models/robot.obj", directionalShader);
    robot->transform->rotateLocal(glm::vec3(0.0f, 90.0f, 0.0f));
    
    DynamicColliderComponent* robotCollider = new DynamicColliderComponent(robot, 0.1f, false, { 0,0 });
    robot->addComponent((Component*)robotCollider);
    DynamicColliderComponent* robotColliderFront = new DynamicColliderComponent(robot, 0.1f, true, { 0.01f, 0.0f });
    robot->addComponent((Component*)robotColliderFront);
    PathFinding pathFinding((Map*)currentMap);
    RobotMovement* robotmovement = new RobotMovement(robot, robot->transform, robotCollider,robotColliderFront, 0.2f, 
        pathFinding, this->TILE_SIZE, { 0,0,1 });
    robot->addComponent((Component*)robotmovement);
    robotmovement->findClosestNode();
    
    player1 = new Entity ("res/models/postacie_zeskalowne/nizej_farmer.obj", directionalShader);
    player2 = new Entity("res/models/postacie_zeskalowne/nizej_farmer_czerwony.obj", directionalShader);
    
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
   

    playerMovement->setRivalPlayerMovement(playerMovement2);
    playerMovement2->setRivalPlayerMovement(playerMovement);
    robotmovement->setPlayers(playerMovement, playerMovement2);

    player1->transform->setLocalPosition(currentMap->getPlayer2Cord());
    player2->transform->setLocalPosition(currentMap->getPlayer1Cord());
    robot->transform->setLocalPosition(currentMap->getRobotCord());

    allTools[0] = new Entity("res/models/lopata.obj", directionalShader);
    DynamicColliderComponent* tool1_collision = new DynamicColliderComponent(allTools[0], 0.05f, true);
    allTools[0]->addComponent((Component*)tool1_collision);
    allTools[0]->addComponent(new Tool(allTools[0], EToolType::Shovel));
    allTools[0]->enableAllComponents(false);

    allTools[1] = new Entity("res/models/motyka.obj", directionalShader);
    DynamicColliderComponent* tool2_collision = new DynamicColliderComponent(allTools[1], 0.05f, true);
    allTools[1]->addComponent((Component*)tool2_collision);
    allTools[1]->addComponent(new Tool(allTools[1], EToolType::Hoe));
    allTools[1]->enableAllComponents(false);
    
    int toolNr = 0;
    std::vector<Entity*> toolstab;
    toolstab.push_back(allTools[0]);
    toolstab.push_back(allTools[1]);
    std::vector<glm::vec3> toolscord = currentMap->getToolsCord();
    for (int i = 0; i < toolscord.size(); i++) {
        toolNr = std::rand() % toolstab.size();
        std::vector<Tool*> vectorTool;
        toolstab[toolNr]->getComponentsByType(&vectorTool);
        vectorTool[0]->setSpawn();
        toolstab[toolNr]->transform->setLocalPosition(toolscord[i]);
        toolstab[toolNr]->enableAllComponents(true);
        toolstab.erase(toolstab.begin() + toolNr);
    }

    //check if it is on the map
    if (robot->transform->getLocalPosition() != glm::vec3(0.f, 0.f, 0.f)) {
        round->addChild(robot);
        robot->enableAllComponents(true);
    }
    else
        robot->enableAllComponents(false);

    currentToolSize = toolscord.size();
    for (int i = 0; i < toolscord.size(); i++) {
        round->addChild(allTools[i]);
        allTools[i]->enableAllComponents(true);
    }
    round->addChild(player1);
    round->addChild(player2);
}

void Round::changeRound(int mapNr)
{
    round->clearChildren();
    currentMap = maps[mapNr];
    currentMap->addHud(hud);
    currentMapManager = mapManagers[mapNr];
    round->addChild(currentMapManager);

    robot->transform->setLocalPosition(currentMap->getRobotCord());

    std::vector<RobotMovement*> robotMovment;
    robot->getComponentsByType(&robotMovment);
    PathFinding pathFinding((Map*)currentMap);
    robotMovment[0]->setPathFinding(pathFinding);
    robotMovment[0]->clearPath();
    robotMovment[0]->findClosestNode();

    std::vector<PlayerMovement*> playerMovment;

    player1->getComponentsByType(&playerMovment);
    playerMovment[0]->dropTool();
    playerMovment[0]->resetSeenTile();
    player1->transform->setLocalPosition(currentMap->getPlayer1Cord());
    playerMovment[0]->cancelAction();

    player2->getComponentsByType(&playerMovment);
    playerMovment[1]->dropTool();
    playerMovment[1]->resetSeenTile();
    player2->transform->setLocalPosition(currentMap->getPlayer2Cord());
    playerMovment[1]->cancelAction();

    allTools[0]->enableAllComponents(false);
    allTools[1]->enableAllComponents(false);
    int toolNr = 0;
    std::vector<Entity*> toolstab;
    toolstab.push_back(allTools[0]);
    toolstab.push_back(allTools[1]);
    std::vector<glm::vec3> toolscord = currentMap->getToolsCord();
    for (int i = 0; i < toolscord.size(); i++) {
        toolNr = std::rand() % toolstab.size();
        std::vector<Tool*> vectorTool;
        toolstab[toolNr]->getComponentsByType(&vectorTool);
        vectorTool[0]->setSpawn();
        toolstab[toolNr]->transform->setLocalPosition(toolscord[i]);
        toolstab[toolNr]->enableAllComponents(true);
        toolstab.erase(toolstab.begin() + toolNr);
    }

    auto skybox = World::getInstance();
    skybox->mapComponent = currentMap;
    skybox->clearReloadLists();

    this->hud->clock->detachClock();
    hud->clock->startClock(roundTime);

    //check if it is on the map
    if (robot->transform->getLocalPosition() != glm::vec3(0.f, 0.f, 0.f)) {
        round->addChild(robot);
        robot->enableAllComponents(true);
    }
    else
    robot->enableAllComponents(false);

    currentToolSize = toolscord.size();
    for (int i = 0; i < toolscord.size(); i++) {
        round->addChild(allTools[i]);
        allTools[i]->enableAllComponents(true);
    }
    round->addChild(player1);
    round->addChild(player2);
}

void Round::regenerateMaps(Model* tileModels, std::string* mapFiles, Shader* directionalShader,
    Shader* pickupShader, Shader* highlightShader)
{
    for (int i = 0; i < 5; i++) {
        mapManagers[i] = new Entity(directionalShader);
        maps[i] = new Map(mapManagers[i], tileModels, mapFiles, TILE_SIZE, pickupShader, highlightShader, i);
        mapManagers[i]->addComponent(maps[i]);
        maps[i]->GenerateMap(i);
    }
    allTools[0]->transform->setLocalPosition(glm::vec3(0.f,0.f,0.f));
    allTools[1]->transform->setLocalPosition(glm::vec3(0.f,0.f,0.f));
    changeRound(0);
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
    if (robot->transform->getLocalPosition() == glm::vec3(0, 0, 0))
        return nullptr;
    return robot;
}

Entity* Round::getMapManager()
{
    return currentMapManager;
}

Map* Round::getMap()
{
    return currentMap;
}

int Round::getToolsSize()
{
    return 2;
}

int Round::getRoundTime()
{
    return roundTime;
}

int Round::getClockSeconds()
{
    return this->hud->clock->getSeconds();
}

Entity** Round::getTools()
{
    Entity* tmp[2];
    for (int i = 0; i < 2; i++) {
        if (allTools[i]->transform->getLocalPosition() != glm::vec3(0, 0, 0))
            tmp[i] = allTools[i];
        else
            tmp[i] = nullptr;
    }
    return tmp;
}