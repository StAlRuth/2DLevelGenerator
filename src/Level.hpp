#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "CompareVector2uLess.hpp"
#include <SFML/Graphics.hpp>
#include <map>

class LevelGenerationAlgorithm;

class Level : public sf::Drawable
{
	friend class LevelGenerationAlgorithm;
	public:
		Level(LevelGenerationAlgorithm* generator = NULL);
		sf::Vector2u getPlayerPos();
		sf::Vector2u getStairPos();
		void movePlayer(char direction);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void updateVertexArray();
		std::map<sf::Vector2u, char, CompareVector2uLess> tiles_;
		sf::VertexArray vertices_;
		LevelGenerationAlgorithm* generator_;
		sf::Vector2u dimensions_;
};
#endif/*LEVEL_HPP*/
