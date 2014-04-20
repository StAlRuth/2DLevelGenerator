#include "MazeGenerationAlgorithm.hpp"
#include "Maze.hpp"
#include <cstdlib>

//Passthrough function to separate interface from implementation
void MazeGenerationAlgorithm::operator()(Maze* level)
{
	generate(level);
	level->updateVertexArray();
}

//Protected interface to allow algorithms to edit the maze.
void MazeGenerationAlgorithm::setMazeTile(Maze* level, sf::Vector2u pos, char type)
{
	level->tiles_[pos] = type;
}

char MazeGenerationAlgorithm::getMazeTile(Maze* level, sf::Vector2u pos)
{
	return level->tiles_[pos];
}

sf::Vector2u MazeGenerationAlgorithm::getMazeDimensions(Maze* level)
{
	return level->dimensions_;
}

//This function creates a single room with the player and staircase placed inside it.
//This also serves as an example Maze Generation Algorithm.
void MazeGenerationAlgorithm::generate(Maze* level)
{
	for(unsigned int i = 0; i < getMazeDimensions(level).x; i++)
	{
		for(unsigned int j = 0; j < getMazeDimensions(level).y; j++)
		{
			if(i == 0 || j == 0 || 
				i == getMazeDimensions(level).x - 1 ||
				j == getMazeDimensions(level).y - 1)
			{
				//Setting a tile is straightforward.
				//The first parameter is the position of the
				//tile provided as an sf::Vector2u, and the
				//second one is the tile type as char.
				//Setting a tile to 1 makes it a wall...
				setMazeTile(level, sf::Vector2u(i, j), 1);
			}
			else
			{
				//Setting it to 0 makes it empty...
				setMazeTile(level, sf::Vector2u(i, j), 0);
			}
		}
	}
	sf::Vector2u playerPos((std::rand() % (getMazeDimensions(level).x - 2)) + 1, 
		(std::rand() % (getMazeDimensions(level).y - 2)) + 1);
	sf::Vector2u stairsPos((std::rand() % (getMazeDimensions(level).x - 2)) + 1,
		(std::rand() % (getMazeDimensions(level).y - 2)) + 1);
	while(playerPos.x == stairsPos.x && playerPos.y == stairsPos.y)
	{
		stairsPos = sf::Vector2u
		((std::rand() % (getMazeDimensions(level).x - 2)) + 1,
		 (std::rand() % (getMazeDimensions(level).y - 2)) + 1);
	}
	//Setting it to 2 makes it the player's location
	//I currently do not have any guards against multiple player locations,
	//so please do not add multiple players to the map, as I don't know
	//what will happen :P
	setMazeTile(level, playerPos, 2);
	//Setting it to 3 makes it the maze exit.
	//Ther emay be multiple exits, but please note that a new maze will be
	//generated once any one of them is reached.
	setMazeTile(level, stairsPos, 3);
}
