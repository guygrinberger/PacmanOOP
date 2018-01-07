#pragma once
#include "State.h"
#include "PacEngine.h"
#include "State.h"
#include "PointerPack.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio.hpp>

class GameState : public State
{
public:
	GameState();
	virtual void run(PointerPack& pack);
	bool loadMap();

private:
	void drawAll();
	void drawWall(int x, int y);
	void drawPill(int x,int y);
	void drawGhosts();
	sf::Text lives, score, level, speed, time;
	sf::Sound raMeod, shalom;
	sf::SoundBuffer sound;
	bool mapOk;
	sf::Texture mAtlas;
	PointerPack * mPack;
	int currentMap, mScore, pacframe;
	std::string mMapNames[8];
	PacEngine mEngine;
	sf::Sprite leftEye, rightEye;
	vector<sf::Sprite> guys;
	sf::Clock mAnimClock;
};