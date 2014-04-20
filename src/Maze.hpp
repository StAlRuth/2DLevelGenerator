#ifndef MAZE_HPP
#define MAZE_HPP
#include "CompareVector2uLess.hpp"
#include <SFML/Graphics.hpp>
#include <map>

class MazeGenerationAlgorithm;

class Maze : public sf::Drawable
{
	friend class MazeGenerationAlgorithm;
	public:
		Maze(MazeGenerationAlgorithm* generator = NULL);
		sf::Vector2u getPlayerPos();
		sf::Vector2u getStairPos();
		void movePlayer(char direction);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void updateVertexArray();
		std::map<sf::Vector2u, char, CompareVector2uLess> tiles_;
		sf::VertexArray vertices_;
		MazeGenerationAlgorithm* generator_;
		sf::Vector2u dimensions_;
};
#endif/*MAZE_HPP*/
