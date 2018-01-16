#include "Save.h"



Save::Save()
{
}

void Save::draw(sf::RenderWindow & window, sf::Sprite & sprite)
{
	sprite.setTextureRect(sf::IntRect(64 * 0, 0, 64, 64));
	window.draw(sprite);
}


Save::~Save()
{
}
