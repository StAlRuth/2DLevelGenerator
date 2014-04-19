#include <SFML/Graphics.hpp>
#include <map>

class CompareVector2uLess
{
	public:
		bool operator()(sf::Vector2u a, sf::Vector2u b) const
		{
			return a.y == b.y ? a.x < b.x : a.y < b.y;
		}
};

class Maze : public sf::Drawable
{
	public:
		Maze();
		sf::Vector2u getPlayerPos();
		void movePlayer(char direction);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void updateVertexArray();
		std::map<sf::Vector2u, char, CompareVector2uLess> tiles_;
		sf::VertexArray vertices_;
		void generate();
};
