#pragma once
#include "PacEntity.h"
#include "PacEvent.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include <deque>
#include "ConstantsGame.h"
#include "Sounds.h"
#include "gameSettings.h"
class ConstantsGame;
struct object
{
	unsigned char symbol;
	sf::Color color;
	PacEntity entity;
	bool isSmart = true;
};

class PacEngine
{
public:
	PacEngine();
	sf::Vector2f getPosition(int who);
	sf::Vector2i publicPacPos;
	float getRotation(int who);	
	void update();
	void setPacDirection(eDirection direction);
	vector<object*> guys;
	sf::Clock cookieTimer,globalClock;
	std::deque<PacEvent> mEventsList;
	std::unique_ptr<Sounds> soundGame;
	vector<std::vector<object*>> gameObjects;// Pacman game objects

	bool loadMap(const std::string& path);
	bool getEvent(PacEvent& event), spaceClicked = false;
	int vertical, horizontal, lastCookieEaten = lastCookie, ghostpos = 0;

private:

	eDirection getNextMove(PacEntity& ent);
	sf::Vector2i getTarg(int who);
	sf::Vector2i startPacPos;
	vector<sf::Vector2i> startPos;
	double overallSpeed = initialSpeed;
	bool changed = false;
	void initSpeed();
	void updatePac();
	void checkCollisions();
	void resetPositions();
	void checkPills();
	void updateGhost(int who);
	void initialByCookie(unsigned char & tmp, int speed, TileType color, double & overallSpeed);
	int whereChanged = -1, speedGreen = 5, speedBlue = 10, mLives, mTotalPills, fetchTileAt(sf::Vector2i pos, sf::Vector2i off), maxBoardSize;
};