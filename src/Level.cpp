#include "Level.hpp"
#include "LevelGenerationAlgorithm.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

Level::Level(LevelGenerationAlgorithm* generator):generator_(generator),dimensions_(15, 15)
{
	if(generator_ == NULL)
	{
		LevelGenerationAlgorithm gen;
		gen(this);
	}
	else
	{
		generator_->operator()(this);
	}
}

sf::Vector2u Level::getPlayerPos()
{
	for(std::map<sf::Vector2u, char, CompareVector2uLess>::iterator
			i = tiles_.begin(); i != tiles_.end(); i++)
	{
		if(i->second == 2)
		{
			return i->first;
		}
	}
	std::cerr << "No player found!\n";
	return sf::Vector2u(0, 0);
}

sf::Vector2u Level::getStairPos()
{
	for(std::map<sf::Vector2u, char, CompareVector2uLess>::iterator
			i = tiles_.begin(); i != tiles_.end(); i++)
	{
		if(i->second == 3)
		{
			return i->first;
		}
	}
	std::cerr << "No stairs found!\n";
	return sf::Vector2u(0, 0);
}

void Level::movePlayer(char direction)
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
		tiles_.clear();
		if(generator_ == NULL)
		{
			LevelGenerationAlgorithm gen;
			gen(this);
		}
		else
		{
			generator_->operator()(this);
		}
		//We leave now so that we don't rerender the Level again
		return;
	}
	updateVertexArray();
}

void Level::updateVertexArray()
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

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(vertices_, states);
}
