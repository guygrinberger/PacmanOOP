#include "CreateMenu.h"
#include "ConstantsMap.h"

CreateMenu::CreateMenu(sf::Texture & iconsTexture)
	:iconsTexture(iconsTexture){
}

CreateMenu::~CreateMenu()
{
}

void CreateMenu::setMenuSprite(){
	iconsSprite.setTexture(iconsTexture);//set the pic
}

unsigned int CreateMenu::getMenuHeight(){
	return menuHeight = static_cast<int>(iconsTexture.getSize().y + TOP_SPACE + BOTTEM_SPACE);	// get menu high
}

ITEM CreateMenu::getSelected() const {
	return ITEM(selectedItem);
}

void CreateMenu::highlight(const sf::Vector2i & mousePos){
	if (mousePos.x <= (int)iconsTexture.getSize().x && mousePos.y <= ICON_SIZE)
		mouseOnIcon = static_cast<int>(floor(mousePos.x / ICON_SIZE));
	else
		mouseOnIcon = -1;
}

void CreateMenu::selectItem(sf::Vector2i mousePos){
	if (mousePos.x <= (int)iconsTexture.getSize().x && mousePos.y <= ICON_SIZE)
		selectedItem = static_cast<int>(floor(mousePos.x / ICON_SIZE));
}

void CreateMenu::drawMenu(sf::RenderWindow & window)
{
	iconsSprite.setPosition(sf::Vector2f(0, TOP_SPACE));// give the menu space on the top
	sf::RectangleShape separator(sf::Vector2f((float)(window.getSize().x), BOTTEM_SPACE));// give the menu space on the bottem
	separator.setFillColor(sf::Color::Red);// set red lines under the menu
	separator.setPosition(0.0f, (float)(iconsTexture.getSize().y + TOP_SPACE));	// Set separator position

	window.draw(iconsSprite);
	window.draw(separator);

	if (mouseOnIcon >= 0) // Check if there is a item to highligh
	{
		sf::RectangleShape highlight(sf::Vector2f((float)(ICON_SIZE), BOTTEM_SPACE));
		highlight.setPosition((float)(ICON_SIZE * mouseOnIcon), (float)(TOP_SPACE + iconsTexture.getSize().y));
		highlight.setFillColor(sf::Color::Red);// when pressed on the sprite
		window.draw(highlight);
	}

	sf::RectangleShape Choosen(sf::Vector2f((float)(ICON_SIZE), BOTTEM_SPACE));// Selected item
	Choosen.setPosition((float)(ICON_SIZE * selectedItem), (float)(TOP_SPACE + iconsTexture.getSize().y));
	Choosen.setFillColor(sf::Color::Green);
	window.draw(Choosen);
}