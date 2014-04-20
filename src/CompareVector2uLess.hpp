#ifndef COMPAREVECTOR2ULESS_HPP
#define COMPAREVECTOR2ULESS_HPP

#include <SFML/System.hpp>

class CompareVector2uLess
{
	public:
		bool operator()(sf::Vector2u a, sf::Vector2u b) const
		{
			return a.y == b.y ? a.x < b.x : a.y < b.y;
		}
};
#endif/*COMPAREVECTOR2ULESS_HPP*/
