#pragma once
#include "Object.h"
#include "ConstantsMap.h"
class Enemy :public Object, public ConstantsMap
{
public:
	Enemy();
	~Enemy();
	void draw(sf::RenderWindow &window, sf::Sprite &sprite);
};

