#pragma once
#include <SFML\Graphics.hpp>
#include <math.h>
#include "ConstantsMap.h"

class CreateMenu : public ConstantsMap
{
public:
	CreateMenu(sf::Texture &menuIcons);// Painter menu constructor
	~CreateMenu();
	unsigned int getMenuHeight(); // get the height of the menu
	void setMenuSprite();// Set menu sprite
	void highlight(const sf::Vector2i &mousePos);// Hight light a menu item on the current mouse location
	void selectItem(sf::Vector2i mousePos);// Choose a menu item
	void drawMenu(sf::RenderWindow &window);// Draw the menu
	ITEM getSelected() const;// Get selected item

private:
	int mouseOnIcon;// mouse clicked on the icon
	unsigned int menuHeight;// Total menu height
	unsigned int selectedItem;// Selected menu item
	sf::Texture &iconsTexture;// Game menu texture
	sf::Sprite iconsSprite;// Game menu sprite
};