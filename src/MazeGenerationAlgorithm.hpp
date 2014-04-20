#ifndef MAZEGENERATIONALGORITHM_HPP
#define MAZEGENERATIONALGORITHM_HPP
#include <SFML/System.hpp>

class Maze;

//This is a base class for a maze generation algorithm.
class MazeGenerationAlgorithm
{
	public:
		//Call this, override generate()
		void operator()(Maze* level);
	protected:
		//Maze operations, because friendship is not inherited.
		void setMazeTile(Maze* level, sf::Vector2u pos, char type);
		char getMazeTile(Maze* level, sf::Vector2u pos);
		sf::Vector2u getMazeDimensions(Maze* level);
	private:
		//Override this, call operator()
		virtual void generate(Maze* level);
};
#endif/*MAZEGENERATIONALGORITHM_HPP*/
