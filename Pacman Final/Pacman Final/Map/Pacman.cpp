#include "Pacman.h"
#include "ConstantsMap.h"

Pacman::Pacman(): Object(PACMAN)
{
}


Pacman::~Pacman()
{
}

void Pacman::draw(sf::RenderWindow & window, sf::Sprite & sprite)
{
	sprite.setTextureRect(sf::IntRect(ICON_SIZE * (int)PACMAN, 0, ICON_SIZE, ICON_SIZE));
	switch (_color) {
	case RED: sprite.setColor(sf::Color::Red);
		break;
	case GREEN: sprite.setColor(sf::Color::Green);
		break;
	default:
		sprite.setColor(sf::Color::Blue);
	}
	window.draw(sprite);
}

