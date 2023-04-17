#include "Map.h"
#include "Model.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "StaticColliderComponent.h"


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
	for (int i = 0; i < data.rows; i++)
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
	for (int i = 0; i < codedMaps[mapNr].rows; i++)
	{
		std::string row = codedMaps[mapNr].codedRow[i];
		for (int j = 0; j < row.length(); j++)
		{
			bool isPassable;
			EState state;
			if (row[j] == ' ') continue;
			switch (row[j])
			{
			case 'x':
				tiles[tileNr].model = &tileModels[EState::Impassable];
				isPassable = false;
				state = EState::Impassable;
				break;
			case 'o':
				tiles[tileNr].model = &tileModels[EState::Overgrown];
				isPassable = true;
				state = EState::Overgrown;
				break;
			case '.':
			default:
				tiles[tileNr].model = &tileModels[EState::Empty];
				isPassable = true;
				state = EState::Empty;
				break;
			}
			tiles[tileNr].shader = parent->shader;
			tiles[tileNr].isModel = true;
			tiles[tileNr].transform->setLocalPosition({ tileSize * i, 0, tileSize * j });
			tilesComp.push_back(TileState(&tiles[tileNr], state, tileModels));
			colliders.push_back(StaticColliderComponent(&tiles[tileNr], { tileSize, tileSize }, isPassable));
			tiles[tileNr].addComponent(&tilesComp.back());
			tiles[tileNr].addComponent(&colliders.back());
			parent->addChild(&tiles[tileNr]);
			tileNr++;
		}
	}

	nrOfTiles = tileNr;
}

Map::Map(Entity* parent, Model* tileModels, std::string* mapFiles, float tileSize, int firstMap)
	:Component(parent), tileModels(tileModels), tileSize(tileSize)
{
	LoadMapsFromFiles(mapFiles);
	GenerateMap(firstMap);
}

void Map::ChangeMap(int mapIndex)
{
	//...
}

int Map::getTilesCount()
{
	return MAX_TILES;
}


void Map::LoadMapsFromFiles(std::string* files)
{
	for (int i = 0; i < NR_OF_MAPS; i++)
	{
		codedMaps[i] = LoadMapFromFile(files[i]);
	}
}
