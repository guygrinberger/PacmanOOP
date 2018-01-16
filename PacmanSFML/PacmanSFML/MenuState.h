#pragma once
#include "State.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>


enum places
{
	PLAY_PLACE,
	MAP_MAKER,
	SCORE_BOARD,
	EXIT,
	MUTE,
	FAIL = -1,
	ADDHIGH = 60,
	ADDLOW = 35,
	MAX_NUMBER_OF_ITEMS = 5
};

class MenuState : public State
{
public:
	MenuState();
	virtual void run(PointerPack& pack);
	void MoveUp();
	void MoveDown();
	bool getSelection(int indexXwindow, int indexYwindow, int indexXmouse, int indexYmouse);
	int getPlace();
	void moveIt(int saveChoose, int whatSelected, int & selectedItemIndex);
	places checkPlace(int indexXwindow, int indexYwindow, int indexXmouse, int indexYmouse);
	bool samePlace = false;
	int saveChoose = 0;

private:
	sf::Text mTitle,
			mPlay,
			mHigh,
			mQuit,
			mMapMaker;
	sf::Sprite yoram;
	sf::Texture mAtlas;
	sf::Text menu[5];
	int selectedItemIndex;
	int whatSelected = 0;
};
