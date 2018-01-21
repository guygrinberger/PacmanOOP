#pragma once
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include "State.h"
#include "PacEngine.h"
#include "gameSettings.h"
#include "State.h"
#include "PointerPack.h"
#include "Sounds.h"
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "ConstantsGame.h"
class GameState : public State
{
public:
	GameState();
	virtual void run(PointerPack& pack); // runs the games - the pac's moves
	void initializingFirstTime(PointerPack& pack); // initializing all the values' game
	bool loadMap();

private:	
	sf::Sound raMeod, shalom; // sounds
	sf::SoundBuffer sound;	// buffer for the sounds
	sf::Texture mAtlas; // conatin the pic of all the characters 
	PointerPack * mPack; // conatin like struct 
	PacEngine mEngine; // conatin the pac & enemies & map
	sf::Sprite leftEye, rightEye; // enemies' eyes
	vector<sf::Sprite> moveables; // vector of the characters
	sf::Clock mAnimClock; // clock for print the characters every few seconds
	vector<std::string> mMapNames; // vector of the all the maps
	sf::Text lives, score, level, speed, time; // texts for the all the menu 
	int currentMap, // to know which map is now
		mScore, // counter for the eaten cookies
		pacframe, // contain integer to know when to print the characters (if 4 - it's every 0.25 sec will print)
		numOfFiles; // index for the vector of the maps
	bool mapOk, // the map have been loaded & the map is ok
		whatDraw = true; // helps to know what to drew (wall or coockies)
	void drawAll(); // draw all the characters
	void drawUnmoving(int x, int y, bool whatDrew); // draws all the characters which not moves
	void drawGhosts(); // draws ghosts 
};