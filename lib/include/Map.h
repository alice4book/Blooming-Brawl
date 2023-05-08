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

class Map :
    public Component
{
private:
    const static int NR_OF_MAPS = 3;
    const static int MAX_ROWS = 10; //like in MapData codedRow
    const static int MAX_COLUMNS = 20;
    const static int MAX_TILES = MAX_ROWS * MAX_COLUMNS;

    float tileSize;
    MapData codedMaps[NR_OF_MAPS];
    int nrOfTiles;

    //spawners
    std::vector <Entity*> spawners;
    Shader* spawnerShader;
    Shader* altShader;

    Entity tiles[MAX_TILES]; //tiles with models one after another
    Entity* allTiles[MAX_ROWS][MAX_COLUMNS] = {}; //tiles with models and empty spaces as NULL
    std::list <TileState> tilesComp;
public:
    std::vector <std::vector <StaticColliderComponent*>> colliders;
private:
    Model* tileModels;

    void LoadMapsFromFiles(std::string* files);
    MapData LoadMapFromFile(std::string file);
    void GenerateMap(int mapNr = -1);
public:
    Map(Entity* parent, Model* tileModels, std::string* mapFiles, float tileSize, Shader* shader, Shader* altShader, int firstMap = -1);
    void ChangeMap(int mapIndex);
    int getTilesCount();

    //Tools
private:
    std::vector<glm::vec3> toolscord;
public:
    std::vector<glm::vec3>  getToolsCord();
};

