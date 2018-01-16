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

class GameState : public State
{
public:
	GameState();
	virtual void run(PointerPack& pack);
	bool loadMap();

private:	
	sf::Sound raMeod, shalom;
	sf::SoundBuffer sound;	
	sf::Texture mAtlas;
	PointerPack * mPack;
	PacEngine mEngine;
	sf::Sprite leftEye, rightEye;
	vector<sf::Sprite> guys;
	sf::Clock mAnimClock;
	vector<std::string> mMapNames;
	sf::Text lives, score, level, speed, time;
	int currentMap, mScore, pacframe, numOfFiles;
	bool mapOk, whatDrew = true;
	void drawAll();
	void drawUnmoving(int x, int y, bool whatDrew);
	void drawGhosts();
};