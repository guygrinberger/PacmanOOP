#pragma once
#include "State.h"
#include <memory>
#include "Map/Painter.h"
#include "ConstantsGame.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

class MenuState : public State
{
public:
	int getPlace();
	MenuState(float width, float height);
	virtual void run(PointerPack& pack);
	bool getSelection(int indexXwindow, int indexYwindow, int indexXmouse, int indexYmouse);
	places checkPlace(int indexXwindow, int indexYwindow, int indexXmouse, int indexYmouse);
	void makeChoose(PointerPack & pack);
	void MoveUp();
	void MoveDown();
	void draw(sf::RenderWindow & window);
	void moveIt(int saveChoose, int whatSelected, int & selectedItemIndex);

private:
	sf::Font font;
	sf::Texture BG;
	sf::Sprite background;	
	std::unique_ptr<MenuState> Menu;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];
	bool samePlace = false;
	unsigned int sizeRow, sizeCol;
	int saveChoose = 0, selectedItemIndex, whatSelected = 0;
};
