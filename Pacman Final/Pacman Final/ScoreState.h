#pragma once
#include "State.h"
#include <SFML/Graphics/Text.hpp>
#include <vector>

class ScoreState : public State
{
public:
	ScoreState(int score);
	virtual ~ScoreState();
	virtual void run(PointerPack& pack); // function for all action on the scoreboard

private:
	std::vector<sf::Text> mLines; // vector of text
	int mWritingTo, // the right line to write the score
		mScore; // score
	std::string mWritingLine;
	sf::Sprite background;
	sf::Texture BG;
};
