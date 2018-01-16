#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Sounds.h"
class StateManager;
class gameWindow;
class PointerPack
{
public:
	sf::RenderWindow * Window;
	StateManager * Manager;
	sf::Font * Font;
	Sounds * Sound;
};