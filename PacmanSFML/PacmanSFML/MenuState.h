#pragma once
#include "State.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

class MenuState : public State
{
public:
	MenuState();
	virtual void run(PointerPack& pack);

private:
	sf::Text mTitle,
			mPlay,
			mHigh,
			mQuit,
			mMapMaker;
	sf::Sprite yoram;
	sf::Texture mAtlas;
};
