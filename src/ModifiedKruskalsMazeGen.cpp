#include "ModifiedKruskalsMazeGen.hpp"
#include "CompareVector2uLess.hpp"
#include <SFML/System.hpp>
#include <map>

void ModifiedKruskalsMazeGen::generate(Maze* level)
{
	//Set up the skeleton - intersections only, everything else is a wall
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			if(i % 2 == 1 && j % 2 == 1)
			{
				setMazeTile(level, sf::Vector2u(i, j), 0);
			}
			else
			{
				setMazeTile(level, sf::Vector2u(i, j), 1);
			}
		}
	}
	//Label every intersection so we can tell when they're connected
	std::map<sf::Vector2u, char, CompareVector2uLess> groups;
	char group = 0;
	for(char i = 1; i < 15; i+=2)
	{
		for(char j = 1; j < 15; j+=2)
		{
			groups[sf::Vector2u(i, j)] = group;
			group++;
		}
	}
	//This loop is the part of the algorithm that actually creates the maze.
	//On every iteration, it will pick two random and adjacent intersections
	//and check if they are connected.  If they are not, then the wall
	//between them will be removed, and the `groups' map will be updated to
	//show the new connection.  After every pass, the groups algorithm is
	//checked to see if all of the intersections are connected and if so, the
	//algoritm finishes.
	while(true)
	{
		//Pick random intersection
		sf::Vector2u int1;
		int1.x = (std::rand()%int(std::floor(15/2))*2)+1;
		int1.y = (std::rand()%int(std::floor(15/2))*2)+1;
		
		//Pick random direction
		sf::Vector2u diff;
		char direction = std::rand() % 4;
		switch(direction)
		{
			case 0:
				diff.x = 0;
				diff.y = 1;
				break;
			case 1:
				diff.x = 1;
				diff.y = 0;
				break;
			case 2:
				diff.x = 0;
				diff.y = -1;
				break;
			case 3:
				diff.x = -1;
				diff.y = 0;
				break;
			default:
				break;
		}
		sf::Vector2u hall(int1.x+diff.x, int1.y+diff.y);
		
		//Check if the path has already been cleared
		if(getMazeTile(level, hall) == 0)
		{
			continue;
		}
		
		char id1 = groups[int1];
		sf::Vector2u int2(int1.x+(2*diff.x), int1.y+(2*diff.y));
		if(groups.count(int2) != 1)
		{
			//We've found the edge, guise!
			continue;
		}
		char id2 = groups[int2];
		if(id1 == id2)
		{
			//Already connected, abort.
			continue;
		}
		//Connect the paths
		for(std::map<sf::Vector2u, char, CompareVector2uLess>::iterator i = groups.begin();
			i != groups.end(); i++)
		{
			if(i->second == id2)
			{
				i->second = id1;
			}
		}
		
		//Clear the path
		setMazeTile(level, hall, 0);
		
		//Check if all intersections are connected
		bool done = true;
		for(std::map<sf::Vector2u, char, CompareVector2uLess>::iterator i = groups.begin();
			i != groups.end(); i++)
		{
			if(i->second != id1)
			{
				done = false;
			}
		}
		if(done == true)
		{
			break;
		}
	}
	setMazeTile(level, sf::Vector2u(1, 1), 2);
	setMazeTile(level, sf::Vector2u((15-2), (15-2)), 3);
}
