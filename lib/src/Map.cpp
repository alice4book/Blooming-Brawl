#include "Map.h"
#include "Model.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "StaticColliderComponent.h"
#include "World.h"
#include "Spawner.h"
#include "HUD.h"


MapData Map::LoadMapFromFile(std::string fileName)
{
	std::fstream file;
	file.open(fileName);
	MapData data;
	file >> data.rows;
	file >> data.impassable;
	file >> data.overgrown;
	std::string line;
	getline(file, line);
	for (int i = data.rows - 1; i >= 0; i--)
	{
		getline(file, data.codedRow[i]);
	}
	file.close();
	return data;
}

void Map::GenerateMap(int mapNr)
{
	if (mapNr == -1)
	{
		mapNr = rand() % NR_OF_MAPS;
	}

	//find random tiles
	glm::vec2 available[MAX_TILES];
	int generationIndex = 0;

	for (int i = 0; i < codedMaps[mapNr].rows; i++)
	{
		std::string row = codedMaps[mapNr].codedRow[i];
		for (int j = 0; j < row.length(); j++)
		{
			std::string row = codedMaps[mapNr].codedRow[i];
			if (row[j] == '?')
			{
				available[generationIndex].x = i;
				available[generationIndex].y = j;
				generationIndex++;
			}
		}
	}

	//generate random tiles
	for (int i = 0; i < codedMaps[mapNr].impassable; i++)
	{
		if (generationIndex == 0) break;
		int chosen = rand() % generationIndex;
		generationIndex--;
		int chosenRow = available[chosen].x;
		int chosenColumn = available[chosen].y;
		codedMaps[mapNr].codedRow[chosenRow][chosenColumn] = 'x';
		available[chosen] = available[generationIndex];
	}
	for (int i = 0; i < codedMaps[mapNr].overgrown; i++)
	{
		if (generationIndex == 0) break;
		int chosen = rand() % generationIndex;
		generationIndex--;
		int chosenRow = available[chosen].x;
		int chosenColumn = available[chosen].y;
		codedMaps[mapNr].codedRow[chosenRow][chosenColumn] = 'o';
		available[chosen] = available[generationIndex];
	}
	while (generationIndex > 0)
	{
		if (generationIndex == 0) break;
		generationIndex--;
		int chosenRow = available[generationIndex].x;
		int chosenColumn = available[generationIndex].y;
		codedMaps[mapNr].codedRow[chosenRow][chosenColumn] = '.';
	}
	
	//create all tiles
	int tileNr = 0;
	emptyTiles = 0;
	std::vector <StaticColliderComponent*> collidersRow;
	for (int i = 0; i < codedMaps[mapNr].rows; i++)
	{
		collidersRow.clear();
		std::string row = codedMaps[mapNr].codedRow[i];
		for (int j = 0; j < row.length(); j++)
		{
			bool isPassable;
			EState state;
			if (row[j] == ' ') {
				allTiles[i][j] = NULL;
				nodes[i][j] = NULL;
				continue;
			}
			switch (row[j])
			{
			case 'x':
				tiles[tileNr].model = &tileModels[EState::Impassable];
				isPassable = false;
				state = EState::Impassable;
				break;
			case 'o':
				tiles[tileNr].model = &tileModels[EState::Overgrown];
				emptyTiles++;
				isPassable = true;
				state = EState::Overgrown;
				break;
			case 't':
				tiles[tileNr].model = &tileModels[EState::Empty];
				emptyTiles++;
				isPassable = true;
				state = EState::Empty;
				break;
			case 's':
				tiles[tileNr].model = &tileModels[EState::Empty];
				emptyTiles++;
				isPassable = true;
				state = EState::Empty;
				if (spawnerShader != nullptr) {
					spawners.push_back(new Entity(spawnerShader));
					parent->addChild(spawners.back());
					spawners.back()->addComponent(new Spawner(spawners.back(), spawnerShader));
				}
				break;
			case '.':
			default:
				tiles[tileNr].model = &tileModels[EState::Empty];
				emptyTiles++;
				isPassable = true;
				state = EState::Empty;
				break;
			}

			tiles[tileNr].altShader = altShader;
			tiles[tileNr].shader = parent->shader;
			tiles[tileNr].isModel = true;
			tiles[tileNr].transform->setLocalPosition({ tileSize * i, 0, tileSize * j }); //({ tileSize * codedMaps[mapNr].rows - tileSize * i, 0, tileSize * j });//({ tileSize * i, 0, tileSize * j });
			if (row[j] == 't') {
				toolscord.push_back(tiles[tileNr].transform->getLocalPosition());
			}
			if (row[j] == 's' && spawners.size() > 0) {
				spawners.back()->transform->setLocalPosition(tiles[tileNr].transform->getLocalPosition());
			}

            tilesComp.push_back(TileState(&tiles[tileNr], state, tileModels, glm::vec2(i,j), this));
            collidersRow.push_back(new StaticColliderComponent(&tiles[tileNr], { tileSize, tileSize }, isPassable, &tilesComp.back()));

			tiles[tileNr].addComponent(&tilesComp.back());
			if(&collidersRow.back() != nullptr)
				tiles[tileNr].addComponent(collidersRow.back());
			parent->addChild(&tiles[tileNr]);
			allTiles[i][j] = &tiles[tileNr];

			nodes[i][j] = &tilesComp.back().node;
			nodes[i][j]->pos.x = i;
			nodes[i][j]->pos.y = j;
			if (tilesComp.back().state == EState::Impassable)
				nodes[i][j]->isPassable = false;

			allTilesComp[i][j] = &tilesComp.back();

			tileNr++;
		}
		colliders.push_back(collidersRow);
	}
	collidersRow.clear();
	nrOfTiles = tileNr;

	//add neighbours
	for (TileState& tile : tilesComp)
	{
		int neighbourNr = 0;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i != 0 || j != 0)
				{
					int neiX = tile.mapPosition.x + i;
					int neiY = tile.mapPosition.y + j;
					if (neiX >= 0 && neiX < MAX_ROWS && neiY >= 0 && neiY < MAX_COLUMNS)
					{
						tile.neighbours[neighbourNr] = allTiles[neiX][neiY];
						tile.node.neighbours[neighbourNr] = nodes[neiX][neiY];
					}
					neighbourNr++;
				}
			}
		}
		//write down neighbours
		/*
		std::cout << "tile " << tile.mapPosition.x << " " << tile.mapPosition.y << std::endl;
		for (int i = 0; i < 8; i++)
		{
			std::vector<TileState*> neigh;
			if (tile.neighbours[i])
			{
				tile.neighbours[i]->getComponentsByType(&neigh);
				std::cout << "     " << i << " " << neigh[0]->mapPosition.x << " " << neigh[0]->mapPosition.y << std::endl ;
			}
			
		}
		*/
	}
}

Map::Map(Entity* parent, Model* tileModels, std::string* mapFiles, float tileSize, Shader* shader, Shader* altShader, int firstMap)
	:Component(parent), tileModels(tileModels), tileSize(tileSize), altShader(altShader), spawnerShader(shader)
{
	LoadMapsFromFiles(mapFiles);
	GenerateMap(firstMap);

    auto world = World::getInstance();
    world->mapComponent = this;
	player1TilesCount = 0;
	player2TilesCount = 0;
}

void Map::ChangeMap(int mapIndex)
{
	//...
}

int Map::getTilesCount()
{
	return MAX_TILES;
}

void Map::addToPlayer1TilesCount(int p1)
{
	player1TilesCount += p1;
	hud->barSize(player1TilesCount, player2TilesCount);
}

void Map::addToPlayer2TilesCount(int p2)
{
	player2TilesCount += p2;
	hud->barSize(player1TilesCount, player2TilesCount);
}

void Map::addHud(HUD* hud)
{
	this->hud = hud;
	if(hud != nullptr)
		hud->setTilesCount(emptyTiles);
}

//WIP
std::vector<TileState*> Map::getPlayerTiles(EPlayerID playerID)
{ 
	// copy all the contents of one list to another
	std::vector<TileState*> playerTileStates;
	std::list<TileState*> listOfTiles;
	TileState* obj;
	while(!listOfTiles.empty()) {
		obj = listOfTiles.back();
		if (playerID == obj->getOwner()) {
			playerTileStates.push_back(obj);
		}
		listOfTiles.pop_back();
	}
	return playerTileStates;
}

void Map::LoadMapsFromFiles(std::string* files)
{
	for (int i = 0; i < NR_OF_MAPS; i++)
	{
		codedMaps[i] = LoadMapFromFile(files[i]);
	}
}

//Tools
std::vector<glm::vec3>  Map::getToolsCord() {
	return toolscord;
}