#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Level.hpp"
#include "LevelGenerationAlgorithm.hpp"
#include "ModifiedKruskalsMazeGen.hpp"

int main()
{
	std::srand(std::time(0));
	sf::RenderWindow app(sf::VideoMode(15*32, 15*32), "Level");
	ModifiedKruskalsMazeGen generator;
	Level level(&generator);
	while(app.isOpen())
	{
		sf::Event event;
		while(app.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				app.close();
			}
			else if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Escape)
				{
					app.close();
				}
				else if(event.key.code == sf::Keyboard::Down)
				{
					level.movePlayer(0);
				}
				else if(event.key.code == sf::Keyboard::Right)
				{
					level.movePlayer(1);
				}
				else if(event.key.code == sf::Keyboard::Up)
				{
					level.movePlayer(2);
				}
				else if(event.key.code == sf::Keyboard::Left)
				{
					level.movePlayer(3);
				}
			}
		}
		app.clear(sf::Color(0, 0, 0));
		app.draw(level);
		app.display();
	}
	return 0;
}
