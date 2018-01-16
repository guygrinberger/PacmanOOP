#include "PacEngine.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

bool enterable(unsigned char number)
{
	if (number == RedWall || number == BlueWall || number == GreenWall)
		return false;

	return true;
}

sf::Vector2i getPosFromNode(int x, int y)
{
	int size = int(gameSettings::getInstance().getScaleSize());
	return sf::Vector2i(x*size + (size / 2), y*size + (size / 2));
}

int fitBetween(int a,int b,int c)
{
	return std::min(std::max(a,b),c);
}

int get2DistanceFromNodes(sf::Vector2i first,sf::Vector2i second)
{
	return (first.x-second.x)*(first.x-second.x)+(first.y-second.y)*(first.y-second.y);
}

eDirection getDirFromVec(sf::Vector2i from)
{
	if(from.x==PlusOne)
		return Right;
	if(from.x==MinusOne)
		return Left;
	if(from.y== PlusOne)
		return Down;
	if(from.y== MinusOne)
		return Up;

	return Right; 
}

PacEngine::PacEngine()  :mLives(3) {
	soundGame = std::make_unique<Sounds>();
}

sf::Vector2f PacEngine::getPosition(int who)
{
	return guys[who]->entity.getFloatPos();
}

float PacEngine::getRotation(int who)
{
	return guys[who]->entity.getFloatRotation();
}

bool PacEngine::loadMap(const std::string& path)
{
	mTotalPills = 0;
	ghostpos = 0;
	startPos.resize(0);
	guys.resize(1);
	guys[Pac] = new object;
	std::ifstream file(path.c_str());

	if (!file.is_open())
		return false;

	//take size from file
	std::string line; //string for every line
	getline(file, line);
	horizontal = atoi(line.c_str());
	getline(file, line);
	vertical = atoi(line.c_str());

	maxBoardSize = std::max(vertical, horizontal);
	gameSettings::getInstance().setSclaeSize(720, maxBoardSize);

	gameObjects.resize(vertical);
	for (unsigned int i = 0; i < gameObjects.size(); i++) 
		gameObjects[i].resize(horizontal);


	for (int y = 0; y < horizontal; ++y)
	{
		for (int x = 0; x < vertical; ++x)
		{
			gameObjects[x][y] = new object;

			if (file.eof()) 
				return false;

			unsigned char current;
			file >> current;

			switch (current)
			{
			case RedPacman:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Red;
				startPacPos = getPosFromNode(x, y);
				publicPacPos = { x, y };
				break;
			case RedElf:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Red;
				++ghostpos;
				startPos.resize(ghostpos);
				guys.resize(ghostpos + 1);
				guys[ghostpos] = new object;
				guys[ghostpos] = gameObjects[x][y];
				startPos[ghostpos - 1] = getPosFromNode(x, y);
				break;
			case RedCookie:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Red;
				++mTotalPills;
				break;
			case RedWall:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Red;
				break;
			case GreenPacman:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Green;
				startPacPos = getPosFromNode(x, y);
				publicPacPos = { x, y };
				break;
			case GreenElf:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Green;
				++ghostpos;
				startPos.resize(ghostpos);
				guys.resize(ghostpos + 1);
				guys[ghostpos] = new object;
				guys[ghostpos] = gameObjects[x][y];
				startPos[ghostpos - 1] = getPosFromNode(x, y);
				break;
			case GreenCookie:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Green;
				++mTotalPills;
				break;
			case GreenWall:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Green;
				break;
			case BluePacman:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Blue;
				startPacPos = getPosFromNode(x, y);
				publicPacPos = { x, y };
				break;
			case BlueElf:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Blue;
				++ghostpos;
				startPos.resize(ghostpos);
				guys.resize(ghostpos + 1);
				guys[ghostpos] = new object;
				guys[ghostpos] = gameObjects[x][y];
				startPos[ghostpos - 1] = getPosFromNode(x, y);
				break;
			case BlueCookie:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Blue;
				++mTotalPills;
				break;
			case BlueWall:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Blue;
				break;
			}
		}
	}

	guys[Pac]->entity.direction = Right;
	guys[Pac]->entity.speed = initialSpeed;

	int random;
	if (guys.size() >= 2) {
		for (int i = 1; i <= ghostpos; ++i)
		{
			guys[i]->entity.speed = initialSpeed;
			random = rand() % 2;
			if (random == 0)
				guys[i]->isSmart = false;
		}

		if (ghostpos != startPos.size())
			return false;
	}
	resetPositions();
	return true;
}

void PacEngine::update()
{
	updatePac();

	for(unsigned int i = 1; i <= startPos.size(); ++i)
		updateGhost(i);

	checkCollisions();
}

bool PacEngine::getEvent(PacEvent& event)
{
	if(!mEventsList.empty())
	{
		event=mEventsList.front();
		mEventsList.pop_front();
		return true;
	}
	else
		return false;
}

void PacEngine::setPacDirection(eDirection direction)
{
		guys[Pac]->entity.nextMove=direction;
}

void PacEngine::updatePac()
{
	sf::Clock globalClock;
	sf::Vector2i update = guys[Pac]->entity.getVectorFromDirection();
	globalClock.restart();
	guys[Pac]->entity.speed = initialSpeed;

	if (spaceClicked)
		guys[Pac]->entity.speed = stop;
	else if(!spaceClicked && lastCookieEaten != lastCookie)
		guys[Pac]->entity.speed = overallSpeed;

	for(int i = 0; i < guys[Pac]->entity.speed; ++i)
	{
		guys[Pac]->entity.position += update;
		if(guys[Pac]->entity.isAtNode())
			break;
	}

	if ((lastCookieEaten == GreenCookie && cookieTimer.getElapsedTime().asSeconds() >= timeGreenCookie) ||
		(lastCookieEaten == BlueCookie && cookieTimer.getElapsedTime().asSeconds() >= timeblueCookie))
			initSpeed();

	if(guys[Pac]->entity.isAtNode())
	{
		sf::Vector2i t = guys[Pac]->entity.getNode();
		unsigned char & tmp = gameObjects[t.x][t.y]->symbol;

		switch(tmp)
		{
		case RedCookie: 
			tmp=Empty;
			mEventsList.push_back(PacEvent(ScoreChange, (ghostpos + 1) * 2));
			checkPills();
			soundGame->eat.play();
			break;
		case GreenCookie:
			initialByCookie(tmp, speedGreen, GreenCookie, overallSpeed);
			break;
		case BlueCookie:
			initialByCookie(tmp, speedBlue, BlueCookie, overallSpeed);
			break;
		}
		if(guys[Pac]->entity.nextMove != None &&
			enterable(fetchTileAt(guys[Pac]->entity.getNode(),guys[Pac]->entity.getVectorFromNext())))
		{
			guys[Pac]->entity.direction=guys[Pac]->entity.nextMove;
			guys[Pac]->entity.nextMove=None;
		}
		if(!enterable(fetchTileAt(guys[Pac]->entity.getNode(),guys[Pac]->entity.getVectorFromDirection())))
			guys[Pac]->entity.direction=None;

		update=guys[Pac]->entity.getVectorFromDirection();
	}
}

void PacEngine::checkCollisions()
{
	int doit = false;
	for(unsigned int i = 1; i <= startPos.size(); ++i)
		if(guys[Pac]->entity.getNode() == guys[i]->entity.getNode())
				doit = true;

	if(!doit) 
		return;

	if(--mLives)
	{
		overallSpeed = initialSpeed;
		mEventsList.push_back(PacEvent(PacDied, mLives));
	}
	else
		mEventsList.push_back(PacEvent(GameOver,0));
	resetPositions();
}

void PacEngine::resetPositions()
{
	lastCookieEaten = lastCookie;
	if (guys.size() >= 2)
		for (unsigned int i = 1; i <= startPos.size(); i++)
			guys[i]->entity.position = startPos[i - 1];

	guys[Pac]->entity.position = startPacPos;
	guys[Pac]->entity.direction = None;
	guys[Pac]->entity.nextMove = None;
}

int PacEngine::fetchTileAt(sf::Vector2i pos,sf::Vector2i off)
{
	int x = pos.x + off.x;
	int y = pos.y + off.y;
	//do not drive out of boundaries
	if (y != fitBetween(0, y, horizontal - 1) || x != fitBetween(0, x, vertical - 1))
	{
		if ((pos.x == (vertical - 1)) || (pos.y = (horizontal - 1)))
			return RedWall;
		if(gameObjects[fitBetween(0, x, vertical - 1)][y - 1]->symbol == RedWall)
			return RedWall;
		if (gameObjects[fitBetween(0, x, vertical - 1)][y - 1]->symbol == BlueWall)
			return BlueWall;
		if (gameObjects[fitBetween(0, x, vertical - 1)][y - 1]->symbol == GreenWall)
			return GreenWall;
	}
	return gameObjects[x][y]->symbol;
}

void PacEngine::checkPills()
{
	int tmp=0;
	for(int y = 0; y < horizontal; ++y) 
		for(int x = 0; x < vertical; ++x)
			if(gameObjects[x][y]->symbol == RedCookie ||gameObjects[x][y]->symbol == BlueCookie ||gameObjects[x][y]->symbol == GreenCookie)
				++tmp;

	if(tmp == 0) 
		mEventsList.push_back(PacEvent(PillsExhausted, 0));
}

void PacEngine::updateGhost(int who)
{
	sf::Vector2i update = guys[who]->entity.getVectorFromDirection();
	for(int i = 0; i < guys[who]->entity.speed; ++i){
		guys[who]->entity.position += update;
		if(guys[who]->entity.isAtNode())
			break;
	}

	if(guys[who]->entity.isAtNode())
	{
		if (guys[who]->isSmart == true)
		{
			//chose next move:
			guys[who]->entity.target = getTarg(who);
			guys[who]->entity.direction = getNextMove(guys[who]->entity);
		}
		else
		{
			int random = rand() % 4;
			guys[who]->entity.direction = eDirection(random);
			// checks if the direction is good
			if (!enterable(fetchTileAt(guys[who]->entity.getNode(), guys[who]->entity.getVectorFromDirection())))
				guys[who]->entity.direction = None;
		}
	}
}

sf::Vector2i PacEngine::getTarg(int who)
{
	return guys[Pac]->entity.getNode();
}

void PacEngine::initSpeed()
{
	guys[Pac]->entity.speed = initialSpeed;
	overallSpeed = initialSpeed;
	lastCookieEaten = lastCookie;
}

void PacEngine::initialByCookie(unsigned char & tmp, int speed, TileType color, double & overallSpeed)
{
	lastCookieEaten = color;
	cookieTimer.restart();
	tmp = Empty;
	mEventsList.push_back(PacEvent(ScoreChange, (ghostpos + 1) * 2));
	checkPills();
	soundGame->eat.play();
	if (lastCookieEaten == GreenCookie)
		overallSpeed += (overallSpeed / speed);
	else
		overallSpeed -= (overallSpeed / speed);
}

eDirection PacEngine::getNextMove(PacEntity& ent)
{
	std::vector<sf::Vector2i> possibleNodes;
	sf::Vector2i tmp=ent.getVectorFromDirection();

	if(enterable(fetchTileAt(ent.getNode(),sf::Vector2i(PlusOne,0))))
		possibleNodes.push_back(ent.getNode()+sf::Vector2i(PlusOne,0));
	if(enterable(fetchTileAt(ent.getNode(),sf::Vector2i(MinusOne,0))))
		possibleNodes.push_back(ent.getNode()+sf::Vector2i(MinusOne,0));
	if(enterable(fetchTileAt(ent.getNode(),sf::Vector2i(0,PlusOne))))
		possibleNodes.push_back(ent.getNode()+sf::Vector2i(0,PlusOne));
	if(enterable(fetchTileAt(ent.getNode(),sf::Vector2i(0, MinusOne))))
		possibleNodes.push_back(ent.getNode()+sf::Vector2i(0, MinusOne));

	for(std::vector<sf::Vector2i>::iterator it=possibleNodes.begin();it!=possibleNodes.end();++it)
	{
		if(*it==(ent.getNode()-ent.getVectorFromDirection()))
		{
			possibleNodes.erase(it);
			break;
		}
	}

	int a = 100000, b = -1;

	for(unsigned int i = 0; i < possibleNodes.size(); ++i)
	{
		if(a>get2DistanceFromNodes(ent.target,possibleNodes[i]))
		{
			a = get2DistanceFromNodes(ent.target,possibleNodes[i]);
			b = i;
		}
	}

	if(b != -1)
		return getDirFromVec(possibleNodes[b] - ent.getNode());
	else
		return getDirFromVec(b * ent.getVectorFromDirection());
}