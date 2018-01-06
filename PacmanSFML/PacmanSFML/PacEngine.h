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

enum eEntsID{Pac=0,Blinky=1,Inky=2,Pinky=3,Clyde=4};

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
		Cherry = '0',
		Pill = '1',
		Booster = '2',
		Wall = '3',
		Tunnel = '7',
		Empty = '8',
		GhostSpawn = '9',
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

	enum ScareStatus{Brave,Scared,Blinking,Dead};
	PacEngine();
	sf::Vector2f getPosition(int who);
	sf::Vector2i publicPacPos;
	float getRotation(int who);
	bool loadMap(const std::string& path);
	void update();
	bool getEvent(PacEvent& event);
	void setPacDirection(PacEntity::eDirection direction);
	void makeWallsMap(sf::RenderTarget & target);
	//16x16 tiles, top 3 and bottom 2 rows go for texts,array in interface=ugly
	//unsigned char mMap[horizontal][vertical];
	ScareStatus getScareStatus(int who)const;
	vector<std::vector<object*>> gameObjects;// Pacman game objects
	vector<object*> guys;
	int vertical, horizontal;
	int lastCookieEaten = 42;
	int ghostpos = 0;
	sf::Clock cookieTimer;
	std::deque<PacEvent> mEventsList;
	
private:
	//methods:
	void updatePac();
	void checkCollisions();
	void resetPositions();
	void checkPills();
	void updateGhost(int who);
	int fetchTileAt(sf::Vector2i pos,sf::Vector2i off);
	//PacEntity::eDirection getNextMove(sf::Vector2f pos,sf::Vector2i targ,PacEntity::eDirection cur);
	sf::Vector2i getTarg(int who);
	//vars:
	sf::Vector2i cherryPos, startPacPos;
	vector<sf::Vector2i> startPos;
	int mLives, mTotalPills, mCherryCountDown, mGhostKillStreak;
	//PacEntity guys[5];
	PacEntity::eDirection getNextMove(PacEntity& ent);
};