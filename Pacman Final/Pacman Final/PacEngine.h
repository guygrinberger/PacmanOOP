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
struct object // struct for each object which moves
{
	unsigned char symbol;
	sf::Color color; 
	PacEntity entity; // direction
	bool isSmart = true; // helps to decide who is smart ghost who not
};

class PacEngine
{
public:
	PacEngine(); // constructor
	sf::Vector2f getPosition(int who); // gets the position of the characters
	sf::Vector2i publicPacPos; 
	float getRotation(int who);	// rotation the pic
	void update(); // calls to update pac & enemies
	void setPacDirection(eDirection direction); // sets direction of the pac
	vector<object*> guys; // vector of the pac & enemies
	sf::Clock cookieTimer,globalClock; // clocks for each cookie & global clock
	std::deque<PacEvent> mEventsList; // queue of events
	std::unique_ptr<Sounds> soundGame; // unique ptr for all sounds
	vector<std::vector<object*>> gameObjects;// Pacman game objects

	bool loadMap(const std::string& path); // laod map
	bool getEvent(PacEvent& event), // gets the event
		 spaceClicked = false; // parameter save if the spcae key has been clicked
	int vertical, horizontal, // size of the map
		lastCookieEaten = lastCookie, // defalut cookie
		ghostpos = 0; // if the map contain 0 enemies

private:

	eDirection getNextMove(PacEntity& ent); // calculate the next move & checks if it's can
	sf::Vector2i getTarg(int who); // helps to the enemies the pac's vector (position)
	sf::Vector2i startPacPos; // knows twhere to position the pac
	vector<sf::Vector2i> startPos; // knows twhere to position the enemies
	double overallSpeed = initialSpeed; // starting speed
	void initSpeed(); // Initializing the speed
	void updatePac(); // updating all the paramets of pac
	void checkCollisions(); // checks if the pac collision with something
	void resetPositions(); // reset the position of the pac & enemies
	void checkPills(); // checks the current cookies in the current moment
	void updateGhost(int who);// updating all the paramets of ghost
	void initialByCookie(unsigned char & tmp, int speed, TileType color, double & overallSpeed); // change the speed's pac by the cookie
	int mLives, mTotalPills,  // calculate the lives , calculate the coockies
		fetchTileAt(sf::Vector2i pos, sf::Vector2i off), // checks if the pac can advance to the current cell - have pac's position & pac's size
		maxBoardSize; //helps to match the pixels of the map to the window
};