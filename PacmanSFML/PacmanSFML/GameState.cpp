#include "GameState.h"
#include <SFML/Window/Event.hpp>
#include "StateManager.h"
#include "ScoreState.h"
#include "SpecialState.h"

GameState::GameState():currentMap(0),mScore(0),mPack(0x0)
{
	pacframe=0;
	mAtlas.loadFromFile("testpacWhite.tga");
	leftEye.setTexture(mAtlas);
	rightEye.setTexture(mAtlas);
	leftEye.setTextureRect(sf::IntRect(0, 26, 4, 4));
	rightEye.setTextureRect(sf::IntRect(0, 26, 4, 4));
	leftEye.setOrigin(2.f, 2.f);
	rightEye.setOrigin(2.f, 2.f);

	mMapNames[0] = "Level1.txt";
	mMapNames[1] = "Level2.txt";
	mMapNames[2] = "Level3.txt";
}

bool GameState::loadMap()
{
	if(currentMap == 3){
		mPack->Manager->pushTop(new ScoreState(mScore));
		return false;
	}

	if(!mEngine.loadMap(mMapNames[currentMap++])){
		mPack->Manager->pushTop(new ErrState("Failed Map Loading"));
		return false;
	}

 	PacEngine mEngine;
	mEngine.loadMap(mMapNames[currentMap - 1]);
	guys.resize(mEngine.ghostpos + 1);
	mEngine.lastCookieEaten = 42;
	for (int i = 0; i < mEngine.ghostpos + 1; ++i){
		guys[i].setTexture(mAtlas);
		guys[i].setOrigin(12.5f, 12.5f);
		guys[i].setTextureRect(sf::IntRect(25, 26, 25, 25));
	}

	guys[0].setTextureRect(sf::IntRect(0, 0, 25, 25));

	for (int i = 1; i <= mEngine.ghostpos; i++)
		guys[i].setColor(mEngine.guys[i]->color);

	guys[0].setColor(mEngine.gameObjects[mEngine.publicPacPos.x][mEngine.publicPacPos.y]->color);

	return true;
}

void GameState::run(PointerPack& pack)
{
	if(!mPack) //first time run() call inits:
	{
		mPack=&pack;
		mapOk=loadMap();
		sound.loadFromFile("bad.ogg");
		raMeod.setBuffer(sound);
		lives.setFont(*pack.Font);
		score.setFont(*pack.Font);
		level.setFont(*pack.Font);
		speed.setFont(*pack.Font);
		time.setFont(*pack.Font);
		lives.setString("Lives:3");
		score.setString("Score:0");
		level.setString("level:1");
		speed.setString("speed:3.000000");
		speed.setString("Time:0");
		lives.setCharacterSize(30);
		score.setCharacterSize(30);
		level.setCharacterSize(30);
		speed.setCharacterSize(20);
		time.setCharacterSize(20);
		lives.setPosition(0.f,600.f);
		score.setPosition(130.f,600.f);
		level.setPosition(300.f, 600.f);
		speed.setPosition(0.f, 650.f);
		time.setPosition(200.f, 650.f);
	}
	sf::Clock clock;
	sf::Event seve;
	PacEvent peve;
	mAnimClock.restart();

	while(mapOk)
	{
		while(pack.Window->pollEvent(seve))
		{
			if(seve.type==sf::Event::Closed)
			{
				pack.Manager->popTop(100);
				return;
			}
			if(seve.type==sf::Event::KeyPressed)
			{
				switch(seve.key.code)
				{
				case sf::Keyboard::P:
					pack.Manager->pushTop(new PauseState());
					return;
					break;
				case sf::Keyboard::Escape:
					pack.Manager->pushTop(new QuitState());
					return;
					break;
				case sf::Keyboard::Space:
					mEngine.spaceClicked = true;
					break;
				case sf::Keyboard::W:
				case sf::Keyboard::Up:
					mEngine.spaceClicked = false;
					mEngine.setPacDirection(PacEntity::Up);
					break;
				case sf::Keyboard::S:
				case sf::Keyboard::Down:
					mEngine.spaceClicked = false;
					mEngine.setPacDirection(PacEntity::Down);
					break;
				case sf::Keyboard::A:
				case sf::Keyboard::Left:
					mEngine.spaceClicked = false;
					mEngine.setPacDirection(PacEntity::Left);
					break;
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
					mEngine.spaceClicked = false;
					mEngine.setPacDirection(PacEntity::Right);
					break;
				}
			}
		}

		while(mEngine.getEvent(peve))
		{
			switch(peve.type)
			{
			case GameOver:
				pack.Manager->pushTop(new ScoreState(mScore));
				return;
				break;
			case PacDied:
				raMeod.play();
				lives.setString("Lives:" + std::to_string(peve.data));
				break;
			case PillsExhausted:
				shalom.play();
				mEngine.mEventsList.push_back(PacEvent(ScoreChange, (guys.size() * 50)));
				mEngine.gameObjects.clear();
				mapOk=loadMap();
				break;
			case ScoreChange:
				mScore+=peve.data;
				score.setString("Score:" + std::to_string(mScore));
				break;
			}
			level.setString("Level:" + std::to_string(currentMap));
		}
		if (!mapOk)
			break;

		pack.Window->clear();

		mEngine.update();
		drawAll();

		pack.Window->draw(score);
		pack.Window->draw(lives);
		pack.Window->draw(level);
		if (mEngine.guys[0]->entity.speed > 3)
			speed.setFillColor(sf::Color::Green);
		 else if (mEngine.guys[0]->entity.speed < 3)
			speed.setFillColor(sf::Color::Red);
		 else
			 speed.setFillColor(sf::Color::White);

		speed.setString("Speed:" + std::to_string(mEngine.guys[0]->entity.speed));
		pack.Window->draw(speed);

		if (mEngine.lastCookieEaten == mEngine.GreenCookie)
				time.setString("Time:" + std::to_string(14 - int(mEngine.cookieTimer.getElapsedTime().asSeconds())));
		else if (mEngine.lastCookieEaten == mEngine.BlueCookie)
				time.setString("Time:" + std::to_string(7 - int(mEngine.cookieTimer.getElapsedTime().asSeconds())));
		else
			time.setString("Time:0");

		pack.Window->draw(time);
		pack.Window->display();
	}
}

void GameState::drawWall(int x, int y)
{
	sf::Sprite spr(mAtlas);
	spr.setTextureRect(sf::IntRect(102, 0, 16, 16));
	spr.setPosition(x*16.f, y*16.f);
	spr.setColor(mEngine.gameObjects[x][y]->color);
	mPack->Window->draw(spr);
}

void GameState::drawPill(int x,int y)
{
	sf::Sprite spr(mAtlas);
	spr.setPosition(x*16.f, y*16.f);
	spr.setTextureRect(sf::IntRect(75, 26, 25, 25));
	spr.setScale(0.5625, 0.5625);
	spr.setColor(mEngine.gameObjects[x][y]->color);
	mPack->Window->draw(spr);
}

void GameState::drawGhosts()
{
	if(mAnimClock.getElapsedTime()>sf::seconds(0.2f))
	{
		mAnimClock.restart();
		++pacframe;
		pacframe %= 4;

		guys[0].setTextureRect(sf::IntRect(25*(3-pacframe),0,25,25));
		guys[0].setScale(0.5625, 0.5625);
		for (int i = 1; i <= mEngine.ghostpos; i++){
			guys[i].setTextureRect(sf::IntRect(25+(pacframe%2)*25,26,25,25));
			guys[i].setScale(0.5625, 0.5625);
		}
	}
}

void GameState::drawAll()
{
	drawGhosts();

	for (int i = 0; i < mEngine.ghostpos + 1; i++)
		guys[i].setPosition(mEngine.getPosition(i));

	guys[0].setRotation(mEngine.getRotation(0));

	for(int x = 0; x < mEngine.vertical; ++x)
		for(int y = 0; y < mEngine.horizontal; ++y){
			switch(mEngine.gameObjects[x][y]->symbol)
			{
			case PacEngine::RedCookie:
				drawPill(x,y);
				break;
			case PacEngine::BlueCookie:
				drawPill(x, y);
				break;
			case PacEngine::GreenCookie:
				drawPill(x, y);
				break;
			case PacEngine::RedWall:
				drawWall(x, y);
				break;
			case PacEngine::BlueWall:
				drawWall(x, y);
				break;
			case PacEngine::GreenWall:
				drawWall(x, y);
				break;
			}
		}

	mPack->Window->draw(guys[0]);
	for (int i = 1; i <= mEngine.ghostpos; i++)
	{
		mPack->Window->draw(guys[i]);
		leftEye.setPosition(guys[i].getPosition());
		rightEye.setPosition(guys[i].getPosition());
		leftEye.move(-4.f,-3.f);
		rightEye.move(4.f,-3.f);
		leftEye.setRotation(mEngine.getRotation(i));
		rightEye.setRotation(mEngine.getRotation(i));
		mPack->Window->draw(leftEye);
		mPack->Window->draw(rightEye);
	}
}
