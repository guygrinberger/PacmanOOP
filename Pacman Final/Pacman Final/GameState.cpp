#include "GameState.h"
#include <SFML/Window/Event.hpp>
#include "StateManager.h"
#include "ScoreState.h"
#include "SpecialState.h"
#include "ConstantsGame.h"
GameState::GameState(): currentMap(0), mScore(0), mPack(0x0) // Initializing the gamestate
{
	pacframe = 0; // Initializing to 0 the moves of pac
	mAtlas.loadFromFile("testpacWhite.png"); // load the pic of the all characters
	mAtlas.setSmooth(true); // make the characters smooth
	// makes pic of the enemies' eyes & place them where needs
	leftEye.setTexture(mAtlas);
	rightEye.setTexture(mAtlas);
	leftEye.setTextureRect(sf::IntRect(0, 26, 4, 4));
	rightEye.setTextureRect(sf::IntRect(0, 26, 4, 4));
	leftEye.setOrigin(2.f, 2.f);
	rightEye.setOrigin(2.f, 2.f);

	// loads all the levels by order
	std::ifstream file;
	std::string level = "Level1.txt"; //starts from level1
	numOfFiles = 1;
	file.open(level);
	// checks if the files is by order
	while(file.good()){
		mMapNames.resize(numOfFiles); // input every good map to the vector of the maps
		mMapNames[numOfFiles - 1] = level;
		file.close(); // close to keep input the maps
		numOfFiles++;
		level = ("Level" + std::to_string(numOfFiles) + ".txt"); // open the next map if it exists
		file.open(level);
	}
	numOfFiles--; // vector stats from 0 place
}

bool GameState::loadMap()
{
	// if the currentmap equal to the size of the vector maps - the game has been finished 
	if(currentMap == numOfFiles){
		mPack->Manager->pushTop(new ScoreState(mScore));
		return false;
	}
	// if the map can be open send message error
	if(!mEngine.loadMap(mMapNames[currentMap++])){
		mPack->Manager->pushTop(new ErrState("Failed Map Loading"));
		return false;
	}

	mEngine.loadMap(mMapNames[currentMap - 1]); // loads the map
	guys.resize(mEngine.ghostpos + 1); // makes the vector of characters by the number of enemies + pac
	mEngine.lastCookieEaten = 42;  // default cookie
	for (int i = 0; i < mEngine.ghostpos + 1; ++i){ // prints all the characters
		guys[i].setTexture(mAtlas);
		guys[i].setOrigin(12.5f, 12.5f);
		guys[i].setTextureRect(sf::IntRect(25, 26, 25, 25));
	}

	guys[Pac].setTextureRect(sf::IntRect(0, 0, 25, 25)); // prints the pac

	for (int i = 1; i <= mEngine.ghostpos; i++) // set color for the enemies
		guys[i].setColor(mEngine.guys[i]->color);

	guys[Pac].setColor(mEngine.gameObjects[mEngine.publicPacPos.x][mEngine.publicPacPos.y]->color); // set the pac's color
	return true; // the map has been loaded 
}

void GameState::run(PointerPack& pack)
{
	if(!mPack) //at the first time the map is initializing
	{
		initializingFirstTime(pack);
		// Gets the appropriate size and fits it to the size of the window
		gameSettings::getInstance().setSclaeSize(720, std::max(mEngine.vertical, mEngine.horizontal)); 
	}
	//sf::Clock clock;
	sf::Event seve; // globals events 
	PacEvent peve; // pac events
	mAnimClock.restart(); // reset the time of the printing characters

	while(mapOk) // till the map has been loaded good
	{
		while(pack.Window->pollEvent(seve))
		{
			if(seve.type==sf::Event::Closed)
			{
				pack.Manager->popTop(clean); // cleans all the stack
				return;
			}
			if(seve.type==sf::Event::KeyPressed)
			{
				switch(seve.key.code)
				{
				case sf::Keyboard::P:
					pack.Manager->pushTop(new PauseState());// prints windows for pause
					return;
				case sf::Keyboard::Escape:
					pack.Manager->pushTop(new QuitState()); // prints windows for end the game
					return;
				case sf::Keyboard::Space:
					mEngine.spaceClicked = true; // helps to stop the pac 
					break;
				case sf::Keyboard::W:
				case sf::Keyboard::Up:
					mEngine.spaceClicked = false;
					mEngine.setPacDirection(Up); // make the pac's direction by the key
					break;
				case sf::Keyboard::S:
				case sf::Keyboard::Down:
					mEngine.spaceClicked = false;
					mEngine.setPacDirection(Down);
					break;
				case sf::Keyboard::A:
				case sf::Keyboard::Left:
					mEngine.spaceClicked = false;
					mEngine.setPacDirection(Left);
					break;
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
					mEngine.spaceClicked = false;
					mEngine.setPacDirection(Right);
					break;
				}
			}
		}

		while(mEngine.getEvent(peve))
		{
			switch(peve.type)
			{
			case GameOver:
				pack.Manager->pushTop(new ScoreState(mScore)); // prints the scoreboard
				return;
			case PacDied:
				raMeod.play();
				lives.setString("Lives:" + std::to_string(peve.data)); // update the lives points
				break;
			case PillsExhausted:
				mEngine.mEventsList.push_back(PacEvent(ScoreChange, (int(guys.size() * 50)))); // calculate the score
				mEngine.gameObjects.clear(); // clean the map from the eaten cookies
				mapOk=loadMap();  // check if the map still ok
				break;
			case ScoreChange:
				mScore+=peve.data; // increase the pac's score
				score.setString("Score:" + std::to_string(mScore)); // update the pac's score
				break;
			}
			level.setString("Level:" + std::to_string(currentMap)); // set the current level
		}
		if (!mapOk) // if the map is not good, end the process
			break;

		pack.Window->clear();

		mEngine.update(); // update the pac & checks Collisions
		drawAll();
		// draws all the values 
		pack.Window->draw(score);
		pack.Window->draw(lives);
		pack.Window->draw(level);
		// checks the pac's speed to know how to fill the font of the speed
		if (mEngine.guys[Pac]->entity.speed > initialSpeed)
			speed.setFillColor(sf::Color::Green);
		else if (mEngine.guys[Pac]->entity.speed < initialSpeed)
			speed.setFillColor(sf::Color::Red);
		else
			speed.setFillColor(sf::Color::White);

		speed.setString("Speed:" + std::to_string(mEngine.guys[Pac]->entity.speed)); // set the current speed of the pac
		pack.Window->draw(speed); // draw the current speed
		// checks the last eaten cookie
		if (mEngine.lastCookieEaten == GreenCookie)
			time.setString("Time:" + std::to_string(timeGreenCookie - int(mEngine.cookieTimer.getElapsedTime().asSeconds())));
		else if (mEngine.lastCookieEaten == BlueCookie)
			time.setString("Time:" + std::to_string(timeBlueCookie - int(mEngine.cookieTimer.getElapsedTime().asSeconds())));
		else
			time.setString("Time: 0");

		pack.Window->draw(time); // draw the current time
		pack.Window->display();
	}
}

void GameState::initializingFirstTime(PointerPack & pack)
{
	mPack = &pack;
	mapOk = loadMap();
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
	lives.setCharacterSize(28);
	score.setCharacterSize(28);
	level.setCharacterSize(28);
	speed.setCharacterSize(20);
	time.setCharacterSize(20);
	lives.setPosition(0.f, 718.f);
	score.setPosition(130.f, 718.f);
	level.setPosition(300.f, 718.f);
	speed.setPosition(0.f, 770.f);
	time.setPosition(200.f, 770.f);
}

void GameState::drawUnmoving(int x, int y, bool whatDrew)
{
	sf::Sprite spr(mAtlas);
	if (whatDrew) // wall or cookie to draw
		spr.setTextureRect(sf::IntRect(75, 26, 25, 25));
	else
		spr.setTextureRect(sf::IntRect(102, 0, 25, 25));
	// make the right size by the windows' size
	spr.setScale(gameSettings::getInstance().getScaleSize() / 25.f, gameSettings::getInstance().getScaleSize() / 25.f);
	spr.setPosition(x*gameSettings::getInstance().getScaleSize(), y*gameSettings::getInstance().getScaleSize());
	spr.setColor(mEngine.gameObjects[x][y]->color); // change the color
	mPack->Window->draw(spr); // draw the object

}

void GameState::drawGhosts()
{
	if(mAnimClock.getElapsedTime()>sf::seconds(0.2f))
	{
		mAnimClock.restart();
		++pacframe;
		pacframe %= 4; // the moving of the enemies & pac
		// make the right size of the pac
		guys[Pac].setTextureRect(sf::IntRect(25*(3-pacframe),0,25,25));
		guys[Pac].setScale(gameSettings::getInstance().getScaleSize() / 25.f, gameSettings::getInstance().getScaleSize() / 25.f);
		for (int i = 1; i <= mEngine.ghostpos; i++)// make the right size of the enemies
		{
			guys[i].setTextureRect(sf::IntRect(25+(pacframe%2)*25,26,25,25));
			guys[i].setScale(gameSettings::getInstance().getScaleSize() / 25.f, gameSettings::getInstance().getScaleSize() / 25.f);
		}
	}
}

void GameState::drawAll()
{
	drawGhosts(); 

	for (int i = 0; i < mEngine.ghostpos + 1; i++)
		guys[i].setPosition(mEngine.getPosition(i)); // set the positions of the enemies

	guys[Pac].setRotation(mEngine.getRotation(x_positive)); // position of the pac at the beginning

	for(int x = 0; x < mEngine.vertical; ++x)
		for(int y = 0; y < mEngine.horizontal; ++y)
		{
			whatDraw = true; // helps to decide what to draw - wall or cookie
			switch(mEngine.gameObjects[x][y]->symbol) // check the symbol
			{
			case RedCookie:
				drawUnmoving(x, y, whatDraw);
				break;
			case BlueCookie:
				drawUnmoving(x, y, whatDraw);
				break;
			case GreenCookie:
				drawUnmoving(x, y, whatDraw);
				break;
			case RedWall:
				whatDraw = false;
				drawUnmoving(x, y, whatDraw);
				break;
			case BlueWall:
				whatDraw = false;
				drawUnmoving(x, y, whatDraw);
				break;
			case GreenWall:
				whatDraw = false;
				drawUnmoving(x, y, whatDraw);
				break;
			}
		}

	mPack->Window->draw(guys[Pac]); // draw the pac
	for (int i = 1; i <= mEngine.ghostpos; i++) // draw the enemies - vector of the enemies starts from cell 1
	{
		mPack->Window->draw(guys[i]); // draw them
		// set the enemies' eyes
		leftEye.setPosition(guys[i].getPosition());
		rightEye.setPosition(guys[i].getPosition());
		// set them by the right size
		leftEye.setScale(gameSettings::getInstance().getScaleSize() / 15.f, gameSettings::getInstance().getScaleSize() / 15.f);
		rightEye.setScale(gameSettings::getInstance().getScaleSize() / 15.f, gameSettings::getInstance().getScaleSize() / 15.f);
		leftEye.move(-(gameSettings::getInstance().getScaleSize() / 5.f), -3.f);
		rightEye.move(gameSettings::getInstance().getScaleSize() / 5.f, -3.f);
		// the directions sets by the pac's direction
		leftEye.setRotation(mEngine.getRotation(i));
		rightEye.setRotation(mEngine.getRotation(i));
		// draw the eyes
		mPack->Window->draw(leftEye);
		mPack->Window->draw(rightEye);
	}
}
