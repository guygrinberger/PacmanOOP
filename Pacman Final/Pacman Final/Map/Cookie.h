#pragma once
#include "Object.h"
#include "ConstantsMap.h"

class Cookie :public Object, public ConstantsMap
{
public:
	Cookie();
	~Cookie();
	void draw(sf::RenderWindow &window, sf::Sprite &sprite);
};

