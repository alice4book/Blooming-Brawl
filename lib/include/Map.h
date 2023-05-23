#pragma once
#include "Component.h"

#include "glm/vec3.hpp"
#include "TileState.h"
#include <string>
#include <list>
#include "Entity.h"

class Entity;
class StaticColliderComponent;
class string;

struct MapData
{
    int rows;
    int impassable;
    int overgrown;
    std::string codedRow[10];
};

class Model;
class Spawner;
class Node;
class HUD;

class Map :
    public Component
{
public:
    const static int NR_OF_MAPS = 3;
    const static int MAX_ROWS = 10; //like in MapData codedRow
    const static int MAX_COLUMNS = 20;
    const static int MAX_TILES = MAX_ROWS * MAX_COLUMNS;
private:
    float tileSize;
    MapData codedMaps[NR_OF_MAPS];
    int nrOfTiles;
    int emptyTiles;
    int player1TilesCount;
    int player2TilesCount;

    //spawners
    std::vector <Entity*> spawners;
    Shader* spawnerShader;
    Shader* altShader;

    Entity tiles[MAX_TILES]; //tiles with models one after another
    Entity* allTiles[MAX_ROWS][MAX_COLUMNS] = {}; //tiles with models and empty spaces as NULL
    std::list <TileState> tilesComp;

    HUD* hud;
    
public:
    TileState* allTilesComp[MAX_ROWS][MAX_COLUMNS] = {};
    std::vector <std::vector <StaticColliderComponent*>> colliders;
    Node* nodes[MAX_ROWS][MAX_COLUMNS] = {}; //for robot path finding
private:
    Model* tileModels;

    void LoadMapsFromFiles(std::string* files);
    MapData LoadMapFromFile(std::string file);
    void GenerateMap(int mapNr = -1);
public:
    Map(Entity* parent, Model* tileModels, std::string* mapFiles, float tileSize, Shader* shader, Shader* altShader, int firstMap = -1);
    void ChangeMap(int mapIndex);

    int getTilesCount();
    void addToPlayer1TilesCount(int p1);
    void addToPlayer2TilesCount(int p2);
    int getPlayer1TilesCount();
    int getPlayer2TilesCount();

    void addHud(HUD* hud);

    //Tools
private:
    std::vector<glm::vec3> toolscord;
public:
    std::vector<glm::vec3>  getToolsCord();
};

