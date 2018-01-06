#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class StateManager;

class PointerPack
{
public:
	sf::RenderWindow * Window;
	StateManager * Manager;
	sf::Font * Font;
};