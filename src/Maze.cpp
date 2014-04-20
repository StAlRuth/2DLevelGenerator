#include "maze.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

Maze::Maze()
{
	generate();
}

void Maze::generate()
{
	//Set up the skeleton - intersections only, everything else is a wall
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			if(i % 2 == 1 && j % 2 == 1)
			{
				tiles_[sf::Vector2u(i, j)] = 0;
			}
			else
			{
				tiles_[sf::Vector2u(i, j)] = 1;
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
	//The goal of this algorithm is to connect all intersections via randomized
	//pathways. In each iteration, it picks a random intersection, and tries to
	//connect it to another intersection that it wasn't already connected to.
	//Once all intersections are connected via some pathway, the algorithm is
	//done.
	while(true)
	{
		//Pick random intersection
		std::cerr << "Picking intersection...\n";
		sf::Vector2u int1;
		int1.x = (std::rand()%int(std::floor(15/2))*2)+1;
		int1.y = (std::rand()%int(std::floor(15/2))*2)+1;
//		int1.x = ((std::rand()%4*2))+1;
//		int1.y = ((std::rand()%4*2))+1;
		
		//Pick random direction
		std::cerr << "Picking direction...\n";
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
		if(tiles_[hall] == 0)
		{
			std::cerr << "Already cleared!!!\n";
			continue;
		}
		
		char id1 = groups[int1];
		sf::Vector2u int2(int1.x+(2*diff.x), int1.y+(2*diff.y));
		if(groups.count(int2) != 1)
		{
			//We've found the edge, guise!
			std::cerr << "Edge here!!!\n";
			continue;
		}
		char id2 = groups[int2];
		if(id1 == id2)
		{
			//Already connected, abort.
			std::cerr << "Already Connected!!!\n";
			continue;
		}
		std::cerr << "Clearing (" << hall.x << ", " << hall.y << ")\n";
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
		tiles_[hall] = 0;
		
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
	tiles_[sf::Vector2u(1, 1)] = 2;
	tiles_[sf::Vector2u((15-2), (15-2))] = 3;
	updateVertexArray();
}

void Maze::movePlayer(char direction)
{
	sf::Vector2u pos(0, 0);
	for(std::map<sf::Vector2u, char, CompareVector2uLess>::iterator i = tiles_.begin(); i != tiles_.end(); i++)
	{
		if(i->second == 2)
		{
			pos.x = i->first.x;
			pos.y = i->first.y;
			break;
		}
	}
	sf::Vector2u movepos(pos);
	switch(direction)
	{
		case 0:
			movepos.y+=1;
			break;
		case 1:
			movepos.x+=1;
			break;
		case 2:
			movepos.y-=1;
			break;
		case 3:
			movepos.x-=1;
			break;
		default:
			break;
	}
	if(tiles_[movepos] == 0)
	{
		tiles_[pos] = 0;
		tiles_[movepos] = 2;
	}
	else if(tiles_[movepos] == 3)
	{
		tiles_[pos] = 0;
		tiles_[movepos] = 2;
		generate();
		//We leave now so that we don't rerender the maze again
		return;
	}
	updateVertexArray();
}

void Maze::updateVertexArray()
{
	vertices_.setPrimitiveType(sf::Quads);
	vertices_.resize(15*15*4);
	for(unsigned char i = 0; i < 15; i++)
	{
		for(unsigned char j = 0; j < 15; j++)
		{
			sf::Vertex* quad = &vertices_[(i + j * 15) * 4];
			quad[0].position = sf::Vector2f(i * 32, j * 32);
			quad[1].position = sf::Vector2f((i + 1) * 32, j * 32);
			quad[2].position = sf::Vector2f((i + 1) * 32, (j + 1) * 32);
			quad[3].position = sf::Vector2f(i * 32, (j + 1) * 32);
			sf::Color color;
			switch(tiles_[sf::Vector2u(i, j)])
			{
				case 3:
					color = sf::Color(  0, 255,   0);
					break;
				case 2:
					color = sf::Color(  0,   0, 255);
					break;
				case 1:
					color = sf::Color(255,   0,   0);
					break;
				case 0:
				default:
					color = sf::Color(  0,   0,   0);
					break;
			}
			quad[0].color = color;
			quad[1].color = color;
			quad[2].color = color;
			quad[3].color = color;
		}
	}
}

void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(vertices_, states);
}
