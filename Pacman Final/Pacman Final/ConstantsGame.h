#pragma once
#include <vector>
#include <memory>
#include <stdlib.h> 
#include <iostream>
#include <fstream>
#include <SFML/Audio.hpp>
#include "SFML\Graphics.hpp"
// size for the text of the main
#define MAX_NUMBER_OF_ITEMS 5 

using std::cin;
using std::cout;
using std::endl;
using std::vector;
// sizes of the windows & backgound pic
enum board {
	board = 800,
	pic = 650
};
// to know alawys where is the pacman in the different vectors
enum eEntsID { Pac = 0 };
// parameters for stack
enum stack
{
	nothing = -1,
	clean = 100,
	one = 1
};
// the variables of the chooses
enum places
{
	PLAY_PLACE,
	MAP_MAKER,
	SCORE_BOARD,
	EXIT,
	MUTE,
	FAIL = -1,
	ADDHIGH = 55,
	ADDLOW = 35
};
// recognize the signs to the characters
enum TileType
{
	Empty = '1',
	RedPacman = '@',
	RedElf = '%',
	RedCookie = '*',
	RedWall = '#',
	GreenPacman = 'W',
	GreenElf = 'T',
	GreenCookie = 'I',
	GreenWall = 'E',
	BluePacman = 'S',
	BlueElf = 'G',
	BlueCookie = 'K',
	BlueWall = 'D'
};
// directions
enum eDirection
{
	Up,
	Down,
	Left,
	Right,
	None = 8,
	x_positive = 0,
	x_negative = 180,
	y_positive = 270,
	y_negative = 90
};
// numbers of pixels to move
enum Direction
{
	PlusOne = 1,
	MinusOne = -1
};

// all effects on the pac after eating cookies
enum cookieffect
{
	initialSpeed = 2, // stating speed of enemies & pac
	lastCookie = 42, // red cookie 
	timeGreenCookie = 14, // time for eaten green cookie
	timeBlueCookie = 7, //  time for eaten blue cookie
	stop = 0, // speed of the pac when he stops
	speedGreen = 5, 
	speedBlue = 10
};


