#ifndef LEVELGENERATIONALGORITHM_HPP
#define LEVELGENERATIONALGORITHM_HPP
#include <SFML/System.hpp>

class Level;

//This is a base class for a Level generation algorithm.
class LevelGenerationAlgorithm
{
	public:
		//Call this, override generate()
		void operator()(Level* level);
	protected:
		//Level operations, because friendship is not inherited.
		void setLevelTile(Level* level, sf::Vector2u pos, char type);
		char getLevelTile(Level* level, sf::Vector2u pos);
		sf::Vector2u getLevelDimensions(Level* level);
	private:
		//Override this, call operator()
		virtual void generate(Level* level);
};
#endif/*LEVELGENERATIONALGORITHM_HPP*/
