#include "GameState.h"
#include <SFML/Window/Event.hpp>
#include "StateManager.h"
#include "ScoreState.h"
//#include "ErrState.h"
#include "SpecialState.h"
//#include "PauseState.h"
//#include "QuitState.h"

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
	//mAtlas.setSmooth(true);
	/*for(int i = 0; i <= mEngine.ghostpos; ++i)
	{
		guys[i].setTexture(mAtlas);
		guys[i].setOrigin(12.5f,12.5f);
		guys[i].setTextureRect(sf::IntRect(25,26,25,25));
	}

	leftEye.setTexture(mAtlas);
	rightEye.setTexture(mAtlas);
	leftEye.setTextureRect(sf::IntRect(0,26,4,4));
	rightEye.setTextureRect(sf::IntRect(0,26,4,4));
	leftEye.setOrigin(2.f,2.f);
	rightEye.setOrigin(2.f,2.f);

	guys[0].setTextureRect(sf::IntRect(0,0,25,25));
	
	for(int i = 1; i <= mEngine.ghostpos; i++)
		guys[i].setColor(sf::Color::Red);*/

	/*guys[0].setColor(sf::Color::Yellow);*/
	//guys[1].setColor(sf::Color::Red);
	//guys[2].setColor(sf::Color::Red);
	//guys[3].setColor(sf::Color::Red);
	//guys[4].setColor(sf::Color::Red);

	mMapNames[0] = "Level1.txt";
	mMapNames[1] = "Level2.txt";
	mMapNames[2] = "Level3.txt";
}

bool GameState::loadMap()
{
	if(currentMap == 3)
	{
		mPack->Manager->pushTop(new ScoreState(mScore));
		return false;
	}

	if(!mEngine.loadMap(mMapNames[currentMap++]))
	{
		mPack->Manager->pushTop(new ErrState("Failed Map Loading"));
		return false;
	}

 	PacEngine mEngine;
	mEngine.loadMap(mMapNames[currentMap - 1]);
	//mEngine.cookieTimer.restart();
	guys.resize(mEngine.ghostpos + 1);
	mEngine.lastCookieEaten = 42;
	for (int i = 0; i < mEngine.ghostpos + 1; ++i)
	{
		guys[i].setTexture(mAtlas);
		guys[i].setOrigin(12.5f, 12.5f);
		guys[i].setTextureRect(sf::IntRect(25, 26, 25, 25));
	}

	guys[0].setTextureRect(sf::IntRect(0, 0, 25, 25));

	for (int i = 1; i <= mEngine.ghostpos; i++)
	{	
		guys[i].setColor(mEngine.guys[i]->color);
	}

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
		sound.loadFromFile("shalom.ogg");
		shalom.setBuffer(sound);
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

	sf::Time delta(sf::seconds(1.f/60.f)),stock;
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
		}//seve
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
		}//peve
		if (!mapOk)
			break;

		pack.Window->clear();
		stock += clock.restart();

		//while(stock >= delta)
		//{
		//	stock -= delta;
			mEngine.update();
		//}
		drawAll();

		pack.Window->draw(score);
		pack.Window->draw(lives);
		pack.Window->draw(level);
		if (mEngine.guys[0]->entity.speed > 3)
			speed.setColor(sf::Color::Green);
		 else if (mEngine.guys[0]->entity.speed < 3)
			speed.setColor(sf::Color::Red);
		 else
			 speed.setColor(sf::Color::White);

		speed.setString("Speed:" + std::to_string(mEngine.guys[0]->entity.speed));
		pack.Window->draw(speed);

		if (mEngine.lastCookieEaten == mEngine.GreenCookie)
		{
				time.setString("Time:" + std::to_string(14 - int(mEngine.cookieTimer.getElapsedTime().asSeconds())));
		}
		else if (mEngine.lastCookieEaten == mEngine.BlueCookie)
		{
				time.setString("Time:" + std::to_string(7 - int(mEngine.cookieTimer.getElapsedTime().asSeconds())));
		}
		else
			time.setString("Time:0");

		pack.Window->draw(time);
		pack.Window->display();
	}
}

void GameState::drawCherry(int x,int y)
{
	sf::Sprite spr(mAtlas);
	spr.setPosition(x*16.f,y*16.f);
	spr.setTextureRect(sf::IntRect(75,26,25,25));
	mPack->Window->draw(spr);
}

void GameState::drawBooster(int x,int y)
{
	sf::Vertex arr[4];
	arr[0].position=sf::Vector2f(16*x+1.f,y*16+1.f);
	arr[1].position=sf::Vector2f(16*x+1.f,y*16+10.f);
	arr[2].position=sf::Vector2f(16*x+10.f,y*16+10.f);
	arr[3].position=sf::Vector2f(16*x+10.f,y*16+1.f);
	mPack->Window->draw(arr,4,sf::Quads);
}

void GameState::drawWall(int x, int y)
{
	sf::Vertex arr[4];
	arr[0].position = sf::Vector2f(16 * x + 3.f, y * 16 + 3.f);
	arr[1].position = sf::Vector2f(16 * x + 3.f, y * 16 + 7.f);
	arr[2].position = sf::Vector2f(16 * x + 7.f, y * 16 + 7.f);
	arr[3].position = sf::Vector2f(16 * x + 7.f, y * 16 + 3.f);

	for (int i = 0; i < 4; i++)
		arr[i].color = mEngine.gameObjects[x][y]->color;

	mPack->Window->draw(arr, 4, sf::Quads);
}

void GameState::drawPill(int x,int y)
{
	sf::Sprite spr(mAtlas);
	spr.setPosition(x*16.f, y*16.f + - 5);
	spr.setTextureRect(sf::IntRect(75, 26, 25, 25));
	spr.setColor(mEngine.gameObjects[x][y]->color);
	mPack->Window->draw(spr);

	//sf::Vertex arr[4];
	//arr[0].position=sf::Vector2f(16*x-6.f,y*16-6.f);
	//arr[1].position=sf::Vector2f(16*x-6.f,y*16+23.f);
	//arr[2].position=sf::Vector2f(16*x+23.f,y*16+23.f);
	//arr[3].position=sf::Vector2f(16*x+23.f,y*16-6.f);

	//for (int i = 0; i < 4; i++)
	//	arr[i].color = mEngine.gameObjects[x][y]->color;

	//mPack->Window->draw(arr,4,sf::Quads);
}

void GameState::drawGhosts()
{
	if(mAnimClock.getElapsedTime()>sf::seconds(0.2f))
	{
		mAnimClock.restart();
		++pacframe;
		pacframe %= 4;

		guys[0].setTextureRect(sf::IntRect(25*(3-pacframe),0,25,25));

		//scared/nonscared color switches,
		for(int i = 1; i <= mEngine.ghostpos; i++)
			guys[i].setColor(mEngine.getScareStatus(i)==PacEngine::Brave ? mEngine.guys[i]->color : sf::Color::Blue);
		/*guys[2].setColor(mEngine.getScareStatus(2)==PacEngine::Brave ? sf::Color::Red : sf::Color::Blue);
		guys[3].setColor(mEngine.getScareStatus(3)==PacEngine::Brave ? sf::Color::Red : sf::Color::Blue);
		guys[4].setColor(mEngine.getScareStatus(4)==PacEngine::Brave ? sf::Color::Red : sf::Color::Blue);*/

		for (int i = 1; i <= mEngine.ghostpos; i++)
		{
			guys[i].setTextureRect(sf::IntRect(25+(pacframe%2)*25,26,25,25));

			if(mEngine.getScareStatus(i)==PacEngine::Blinking)//blinking color switches
			{
				guys[i].setColor(pacframe % i ? sf::Color::White : sf::Color::Blue);
			}
		}
	}
}

void GameState::drawAll()
{
	drawGhosts();
	mEngine.makeWallsMap(*mPack->Window);

	for (int i = 0; i < mEngine.ghostpos + 1; i++)
	{
		guys[i].setPosition(mEngine.getPosition(i));
	}

	guys[0].setRotation(mEngine.getRotation(0));

	for(int x = 0; x < mEngine.vertical; ++x)
		for(int y = 0; y < mEngine.horizontal; ++y)
		{
			switch(mEngine.gameObjects[x][y]->symbol)
			{
			case PacEngine::Cherry:
				drawCherry(x,y);
				break;
			case PacEngine::Booster:
				drawBooster(x,y);
				break;
			case PacEngine::RedCookie:
				drawPill(x,y);
				break;
			case PacEngine::BlueCookie:
				drawPill(x, y);
				break;
			case PacEngine::GreenCookie:
				drawPill(x, y);
				break;
			}
		}

	mPack->Window->draw(guys[0]);
	//fake pacmans on the right and left of map for tunnel smooth passing
	/*guys[0].move(16.f*28.f,0.f);
	mPack->Window->draw(guys[0]);
	guys[0].move(-2.f*16.f*28.f,0.f);
	mPack->Window->draw(guys[0]);*/
	for (int i = 1; i <= mEngine.ghostpos; i++)
	{
		if (mEngine.getScareStatus(i) != PacEngine::Dead)
		{
			mPack->Window->draw(guys[i]);
		}

		leftEye.setPosition(guys[i].getPosition());
		rightEye.setPosition(guys[i].getPosition());
		leftEye.move(-4.f,-3.f);
		rightEye.move(4.f,-3.f);

		if(mEngine.getScareStatus(i)==PacEngine::Brave)
		{
			leftEye.setRotation(mEngine.getRotation(i));
			rightEye.setRotation(mEngine.getRotation(i));
		}
		else
		{
			leftEye.setRotation(90);//90 is down in 2d coord system with reversed y axis
			rightEye.setRotation(90);
		}
		mPack->Window->draw(leftEye);
		mPack->Window->draw(rightEye);
	}
}
