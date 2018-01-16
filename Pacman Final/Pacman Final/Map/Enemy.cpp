#include "Enemy.h"

Enemy::Enemy(): Object(ENEMY)
{
}


Enemy::~Enemy()
{
}

void Enemy::draw(sf::RenderWindow & window, sf::Sprite & sprite)
{
	sprite.setTextureRect(sf::IntRect(ICON_SIZE * (int)ENEMY, 0, ICON_SIZE, ICON_SIZE));
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
