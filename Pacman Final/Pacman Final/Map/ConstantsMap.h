#pragma once
#include <SFML\Graphics.hpp>

#define LEVELS_SIZE 3

enum PLACE {
	ZERO,
	ONE,
	TWO,
	THREE,
	ERROR = -1
};

enum ITEM {
	SAVE,
	CLEAR,
	ERASE,
	PACMAN,
	ENEMY,
	COOKIE,
	WALL,
	RED,
	GREEN,
	BLUE
};

class ConstantsMap
{
protected:
	const int ICON_SIZE = 64;// Texture size	
	const float TOP_SPACE = 2;// space in the top
	const float BOTTEM_SPACE = 2;// space in the bottemB
};

