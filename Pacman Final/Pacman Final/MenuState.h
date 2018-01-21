#pragma once
#include "State.h"
#include <memory>
#include "Map/Painter.h"
#include "ConstantsGame.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include "SpecialState.h"

class MenuState : public State
{
public:
	int getPlace(); // return the current choice
	MenuState(float width, float height); // constractor for the menu
	virtual void run(PointerPack& pack); // run all the process
	bool getSelection(float indexXmouse, float indexYmouse, bool isClicked); // gets the selected button, when the player click
	places checkPlace(float indexXmouse, float indexYmouse); // returns the right button
	void makeChoose(PointerPack & pack); // checks the 
	void MoveUp(); // makes the player's choice by pressing the up key
	void MoveDown(); // makes the player's choice by pressing the down key
	void draw(sf::RenderWindow & window); // draws the menu
	void moveIt(int saveChoose, int whatSelected, int & selectedItemIndex); // moves to the right button if the player clicked on the moves

private:
	sf::Font font;
	sf::Texture BG;
	sf::Sprite background;
	std::unique_ptr<MenuState> Menu; // helps to call for some functions & parameters 
	sf::Text menu[MAX_NUMBER_OF_ITEMS]; // array of text for buttons
	bool samePlace = false; // helps to avoid wrong click & same clicks
	unsigned int sizeRow, sizeCol; // ize of the windows
	int saveChoose = 0, // knows what was the last choice
		selectedItemIndex,  // counter to know where the player on the text menu
		whatSelected = 0, // knows where to move the green color of the buttons
		numOfFiles; // counter for the maps

					// parameters for the animation && function for that
	float geraX = -200, guyX = 520;
	sf::Texture geraT, guyT;
	sf::Sprite gera, guy;
	bool geraInPlace = false, guyInPlace = false;
	void justFun();
	bool isClicked = false;
};