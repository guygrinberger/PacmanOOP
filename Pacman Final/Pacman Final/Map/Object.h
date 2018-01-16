#pragma once
#include <SFML\Graphics.hpp>
#include "CreateMenu.h"
#include "ConstantsMap.h"
class Object
{
public:
	Object(ITEM symbol);
	~Object();
	void setColor(ITEM color);
	void setSymbol(ITEM symbol);
	ITEM getSymbol();
	ITEM getColor();
	virtual void draw(sf::RenderWindow &window, sf::Sprite &sprite) = 0;

protected:
	ITEM _symbol;
	ITEM _color;
	sf::Vector2i index;	// Cell index of an object to avoid wrong inputs
};

