#pragma once
#include "ConstantsGame.h"
class Sounds
{
public:
	Sounds();
	~Sounds();
	sf::SoundBuffer buffer, buffer1, buffer2 , buffer3;
	sf::Sound buttom, welcome, bye, eat;
	sf::Music Background;
	bool muted = false;
};

