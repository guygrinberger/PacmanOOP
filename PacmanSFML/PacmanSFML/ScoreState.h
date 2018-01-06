#pragma once
#include "State.h"
#include <SFML/Graphics/Text.hpp>
#include <vector>

class ScoreState : public State
{
public:
	ScoreState(int score);
	virtual ~ScoreState();
	virtual void run(PointerPack& pack);

private:
	std::vector<sf::Text> mLines;
	int mWritingTo,
		mScore;
	std::string mWritingLine;

};
