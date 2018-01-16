#include "gameSettings.h"



gameSettings & gameSettings::getInstance() {
	static gameSettings instance;
	return instance;
}

void gameSettings::setSclaeSize(int maxWindowSize, int maxBoardSize)
{
	scaleSize = float(maxWindowSize) / float(maxBoardSize);
}

float gameSettings::getScaleSize()
{
	return scaleSize;
}

gameSettings::gameSettings()
{
}


gameSettings::~gameSettings()
{
}


