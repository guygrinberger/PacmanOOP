#pragma once
#include <SFML/System/Vector2.hpp>
#include "ConstantsGame.h"
#include "gameSettings.h"
class PacEntity
{
public:
	double speed;
	sf::Vector2i position;
	sf::Vector2i target;
	eDirection direction;
	eDirection nextMove;
	int size;

	sf::Vector2f getFloatPos(){return sf::Vector2f(position);}

	float getFloatRotation()
	{
		switch(direction) // get the direction
		{
		default:
		case Right: return (float)x_positive;
		case Left: return (float)x_negative;
		case Up: return (float)y_positive;
		case Down: return (float)y_negative;
		}
	}

	bool isAtNode()
	{ 
		size = int(gameSettings::getInstance().getScaleSize());
		return ((((position.x - (size / 2)) % size) == 0) && (((position.y - (size / 2)) % size) == 0));
	}

	sf::Vector2i getVectorFromDirection()
	{
		switch(direction) // how to move
		{
		case Left:return 
			sf::Vector2i(MinusOne,0);
		case Right:return 
			sf::Vector2i(PlusOne,0);
		case Up:return 
			sf::Vector2i(0, MinusOne);
		case Down:return 
			sf::Vector2i(0, PlusOne);
		}
		return sf::Vector2i(0,0);
	}

	sf::Vector2i getVectorFromNext()
	{
		switch(nextMove) // get the next move
		{
		case Left:return 
			sf::Vector2i(MinusOne,0);
		case Right:return 
			sf::Vector2i(PlusOne,0);
		case Up:return
			sf::Vector2i(0,MinusOne);
		case Down:return 
			sf::Vector2i(0, PlusOne);
		}
		return sf::Vector2i(0,0);
	}

	sf::Vector2i getNode(){ return position / size;} // knows the right cell
};
