#pragma once
#include <vector>
#include <memory>
#include <stdlib.h> 
#include <iostream>
#include <fstream>
#include <SFML/Audio.hpp>
#include "SFML\Graphics.hpp"

#define MAX_NUMBER_OF_ITEMS 5
using std::cin;
using std::cout;
using std::endl;
using std::vector;

enum board {
	board = 800,
	pic = 650
};
enum eEntsID { Pac = 0 };

enum places
{
	PLAY_PLACE,
	MAP_MAKER,
	SCORE_BOARD,
	EXIT,
	MUTE,
	FAIL = -1,
	ADDHIGH = 60,
	ADDLOW = 35
};
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
enum Direction
{
	PlusOne = 1,
	MinusOne = -1
};
enum cookieffect
{
	initialSpeed = 2,
	lastCookie = 42,
	timeGreenCookie =14,
	timeblueCookie = 7,
	stop = 0
};


