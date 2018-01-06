#pragma once
#include "State.h"
#include <SFML/Graphics/Text.hpp>

class MenuState : public State
{
public:
	MenuState();
	virtual void run(PointerPack& pack);

private:
	sf::Text mTitle,
			 mPlay,
			 mHigh,
			 mQuit;
};
