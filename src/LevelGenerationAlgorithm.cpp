#include "LevelGenerationAlgorithm.hpp"
#include "Level.hpp"
#include <cstdlib>

//Passthrough function to separate interface from implementation
void LevelGenerationAlgorithm::operator()(Level* level)
{
	generate(level);
	level->updateVertexArray();
}

//Protected interface to allow algorithms to edit the Level.
void LevelGenerationAlgorithm::setLevelTile(Level* level, sf::Vector2u pos, char type)
{
	level->tiles_[pos] = type;
}

char LevelGenerationAlgorithm::getLevelTile(Level* level, sf::Vector2u pos)
{
	return level->tiles_[pos];
}

sf::Vector2u LevelGenerationAlgorithm::getLevelDimensions(Level* level)
{
	return level->dimensions_;
}

//This function creates a single room with the player and staircase placed inside it.
//This also serves as an example Level Generation Algorithm.
void LevelGenerationAlgorithm::generate(Level* level)
{
	for(unsigned int i = 0; i < getLevelDimensions(level).x; i++)
	{
		for(unsigned int j = 0; j < getLevelDimensions(level).y; j++)
		{
			if(i == 0 || j == 0 || 
				i == getLevelDimensions(level).x - 1 ||
				j == getLevelDimensions(level).y - 1)
			{
				//Setting a tile is straightforward.
				//The first parameter is the position of the
				//tile provided as an sf::Vector2u, and the
				//second one is the tile type as char.
				//Setting a tile to 1 makes it a wall...
				setLevelTile(level, sf::Vector2u(i, j), 1);
			}
			else
			{
				//Setting it to 0 makes it empty...
				setLevelTile(level, sf::Vector2u(i, j), 0);
			}
		}
	}
	sf::Vector2u playerPos((std::rand() % (getLevelDimensions(level).x - 2)) + 1, 
		(std::rand() % (getLevelDimensions(level).y - 2)) + 1);
	sf::Vector2u stairsPos((std::rand() % (getLevelDimensions(level).x - 2)) + 1,
		(std::rand() % (getLevelDimensions(level).y - 2)) + 1);
	while(playerPos.x == stairsPos.x && playerPos.y == stairsPos.y)
	{
		stairsPos = sf::Vector2u
		((std::rand() % (getLevelDimensions(level).x - 2)) + 1,
		 (std::rand() % (getLevelDimensions(level).y - 2)) + 1);
	}
	//Setting it to 2 makes it the player's location
	//I currently do not have any guards against multiple player locations,
	//so please do not add multiple players to the map, as I don't know
	//what will happen :P
	setLevelTile(level, playerPos, 2);
	//Setting it to 3 makes it the Level exit.
	//Ther emay be multiple exits, but please note that a new Level will be
	//generated once any one of them is reached.
	setLevelTile(level, stairsPos, 3);
}
