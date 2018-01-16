#pragma once
#include "Object.h"
#include "ConstantsMap.h"

class Wall :public Object, public ConstantsMap
{
public:
	Wall();
	~Wall();
	void draw(sf::RenderWindow &window, sf::Sprite &sprite);
};

