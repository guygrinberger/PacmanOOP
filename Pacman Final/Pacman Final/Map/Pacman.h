#pragma once
#include "Object.h"
#include "ConstantsMap.h"

class Pacman : public Object, public ConstantsMap
{
public:
	Pacman();
	~Pacman();
	void draw(sf::RenderWindow &window, sf::Sprite &sprite);
};

