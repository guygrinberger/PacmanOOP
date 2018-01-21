#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Sounds.h"
class StateManager;
class gameWindow;
class PointerPack
{
public:
	sf::RenderWindow * Window; // pointer for windows- make sure the windows is just one
	StateManager * Manager; // holds all the windows with the all
	sf::Font * Font; // pointer for all strings
	Sounds * Sound; // // pointer for all sounds
};