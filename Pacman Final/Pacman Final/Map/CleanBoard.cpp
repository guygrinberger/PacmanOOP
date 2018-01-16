#include "CleanBoard.h"



CleanBoard::CleanBoard()
{
}


void CleanBoard::draw(sf::RenderWindow & window, sf::Sprite & sprite)
{
	sprite.setTextureRect(sf::IntRect(64 * 1, 0, 64, 64));
	window.draw(sprite);
}


CleanBoard::~CleanBoard()
{
}
