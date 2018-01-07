#pragma once
#include "PacEntity.h"
#include "PacEvent.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include <deque>
#include <vector>
using std::vector;

enum eEntsID { Pac = 0 };

struct object
{
	unsigned char symbol;
	sf::Color color;
	PacEntity entity;
};

class PacEngine
{
public:
	enum TileType
	{
		Empty = '8',
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

	PacEngine();
	sf::Vector2f getPosition(int who);
	sf::Vector2i publicPacPos;
	float getRotation(int who);
	bool loadMap(const std::string& path);
	void update();
	bool getEvent(PacEvent& event);
	void setPacDirection(PacEntity::eDirection direction);
	void makeWallsMap(sf::RenderTarget & target);
	vector<std::vector<object*>> gameObjects;// Pacman game objects
	vector<object*> guys;
	int vertical, horizontal;
	int lastCookieEaten = 42;
	int ghostpos = 0;
	sf::Clock cookieTimer;
	std::deque<PacEvent> mEventsList;
	bool spaceClicked = false;
	
private:
	void updatePac();
	void checkCollisions();
	void resetPositions();
	void checkPills();
	void updateGhost(int who);
	int fetchTileAt(sf::Vector2i pos,sf::Vector2i off);
	PacEntity::eDirection getNextMove(PacEntity& ent);
	sf::Vector2i getTarg(int who);
	sf::Vector2i startPacPos;
	vector<sf::Vector2i> startPos;
	int mLives, mTotalPills;
	double overallSpeed = 3;
};