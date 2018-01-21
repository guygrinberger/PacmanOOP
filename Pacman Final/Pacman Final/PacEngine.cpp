#include "PacEngine.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <queue>

bool enterable(unsigned char number) // checks if the cell contain wall
{
	if (number == RedWall || number == BlueWall || number == GreenWall)
		return false;

	return true;
}

sf::Vector2i getPosFromNode(int x, int y) // return cell in pixels size
{
	int size = int(gameSettings::getInstance().getScaleSize()); // size of the windows
	return sf::Vector2i(x*size + (size / 2), y*size + (size / 2)); // calculate
}

int fitBetween(int a,int b,int c) // if the pac & enemies can advance 
{
	return std::min(std::max(a,b),c);
}

int get2DistanceFromNodes(sf::Vector2i first,sf::Vector2i second) // return the distance from 2 points
{
	return (first.x - second.x) * (first.x - second.x) + (first.y  -second.y) * (first.y-second.y);
}

eDirection getDirFromVec(sf::Vector2i from) // gets the direction from the vector & returns the right direction
{
	if(from.x == PlusOne)
		return Right;
	if(from.x == MinusOne)
		return Left;
	if(from.y == PlusOne)
		return Down;
	if(from.y == MinusOne)
		return Up;

	return Right; 
}

PacEngine::PacEngine()  :mLives(3) {
	soundGame = std::make_unique<Sounds>(); // Initializing the pointer
}

sf::Vector2f PacEngine::getPosition(int who)
{
	return moveables[who]->entity.getFloatPos();
}

float PacEngine::getRotation(int who)
{
	return moveables[who]->entity.getFloatRotation();
}

std::vector<sf::Vector2i> PacEngine::neigbors(sf::Vector2i current)
{
	std::vector<sf::Vector2i> neighbors;
	std::vector<sf::Vector2i> offsets;
	offsets.push_back({ current.x + 1, current.y });
	offsets.push_back({ current.x - 1, current.y });
	offsets.push_back({ current.x, current.y + 1});
	offsets.push_back({ current.x, current.y - 1});
	for (int i = 0; i < offsets.size(); i++)
	{
		if (offsets[i].x >= 0 && offsets[i].x < vertical && offsets[i].y >= 0 && offsets[i].y < horizontal)
			if (enterable(gameObjects[offsets[i].x][offsets[i].y]->symbol))
				neighbors.push_back(offsets[i]);
	}
	return neighbors;
}

void PacEngine::updateBFS(sf::Vector2i source)
{
	bfsMap.assign(horizontal, std::vector<bfsNode>(vertical));
	bfsMap[source.y][source.x].isUsed = true;
	bfsMap[source.y][source.x].distance = 0;
	std::queue<sf::Vector2i> bfsQueue;
	bfsQueue.push(source);
	while (!bfsQueue.empty()) {
		auto current = bfsQueue.front();
		bfsQueue.pop();
		if (current.x == 9 && current.y == 1)
			auto a = 1;

		auto currentNeighbors = neigbors(current);

		for (int i = 0; i < currentNeighbors.size(); i++)
		{
			if (!bfsMap[currentNeighbors[i].y][currentNeighbors[i].x].isUsed) {
				bfsMap[currentNeighbors[i].y][currentNeighbors[i].x].isUsed = true;
				bfsMap[currentNeighbors[i].y][currentNeighbors[i].x].pi = current;
				bfsMap[currentNeighbors[i].y][currentNeighbors[i].x].distance = bfsMap[current.y][current.x].distance + 1;
				bfsQueue.push(currentNeighbors[i]);
			}
		}
	}
}

bool PacEngine::loadMap(const std::string& path)
{
	mTotalPills = 0;
	ghostpos = 0;
	startPos.resize(0);
	moveables.resize(1);
	moveables[Pac] = new object;
	std::ifstream file(path.c_str());

	if (!file.is_open())
		return false;

	//take size from file
	std::string line; //string for every line
	getline(file, line);
	horizontal = atoi(line.c_str());
	getline(file, line);
	vertical = atoi(line.c_str());
	

	maxBoardSize = std::max(vertical, horizontal); // checks the bigger
	gameSettings::getInstance().setSclaeSize(720, maxBoardSize); // makes the map match to windows' size

	gameObjects.resize(vertical);
	for (unsigned int i = 0; i < gameObjects.size(); i++) // input the objects in the vector
		gameObjects[i].resize(horizontal);


	for (int y = 0; y < horizontal; ++y) // input the objects from the file 
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
				startPacPos = getPosFromNode(x, y); // the start pac position by the right pixels
				publicPacPos = { x, y }; // to know everytime where is the pac
				break;
			case RedElf:
				gameObjects[x][y]->symbol = current;
				gameObjects[x][y]->color = sf::Color::Red;
				++ghostpos; // increase the number of the enemies
				startPos.resize(ghostpos); // resize the size of the enemies
				moveables.resize(ghostpos + 1);
				moveables[ghostpos] = new object;
				moveables[ghostpos] = gameObjects[x][y];
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
				moveables.resize(ghostpos + 1);
				moveables[ghostpos] = new object;
				moveables[ghostpos] = gameObjects[x][y];
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
				moveables.resize(ghostpos + 1);
				moveables[ghostpos] = new object;
				moveables[ghostpos] = gameObjects[x][y];
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

	moveables[Pac]->entity.direction = Right; // defualt direction's pac
	int random;
	if (moveables.size() >= 2) { // 2 - in the 0 cell is the pac & if i have at less one ghost
		for (int i = 0; i <= ghostpos; ++i) // choose if the enemy is smart or not
		{
			moveables[i]->entity.speed = initialSpeed;
			random = rand() % 2;
			if (random == 0)
				moveables[i]->isSmart = false;
		}

		if (ghostpos != startPos.size()) // if false the map is not ok
			return false;
	}
	resetPositions(); // starting position
	return true;
}

void PacEngine::update()
{
	updatePac();

	for(unsigned int i = 1; i <= startPos.size(); ++i) // update each ghost alone
		updateGhost(i);

	checkCollisions();
}

bool PacEngine::getEvent(PacEvent& event)
{
	if(!mEventsList.empty())
	{
		event = mEventsList.front(); // event is the first event in the queue
		mEventsList.pop_front(); // pop this event 
		return true;
	}
	else
		return false;
}

void PacEngine::setPacDirection(eDirection direction)
{
		moveables[Pac]->entity.nextMove=direction;
}

void PacEngine::updatePac()
{
	sf::Clock globalClock;
	sf::Vector2i update = moveables[Pac]->entity.getVectorFromDirection();
	globalClock.restart();
	moveables[Pac]->entity.speed = initialSpeed;

	if (spaceClicked) // if the space kep has been pressed
		moveables[Pac]->entity.speed = stop;
	else if(!spaceClicked && lastCookieEaten != lastCookie) // if it's not the red cookie
		moveables[Pac]->entity.speed = overallSpeed;

	for(int i = 0; i < moveables[Pac]->entity.speed; ++i) // moving the pac by the current speed to make the moving smooth
	{
		moveables[Pac]->entity.position += update;
		if(moveables[Pac]->entity.isAtNode())
			break;
	}
	// if the cookie is not red and the time of cookies' influence had been finished
	if ((lastCookieEaten == GreenCookie && cookieTimer.getElapsedTime().asSeconds() >= timeGreenCookie) ||
		(lastCookieEaten == BlueCookie && cookieTimer.getElapsedTime().asSeconds() >= timeBlueCookie))
			initSpeed();

	if(moveables[Pac]->entity.isAtNode()) // if the pac in the right pixels 
	{
		sf::Vector2i t = moveables[Pac]->entity.getNode(); // gets the pixels - like a temp
		unsigned char & tmp = gameObjects[t.x][t.y]->symbol;

		switch(tmp)
		{
		case RedCookie: 
			tmp=Empty; // the cookie has been eaten so make the current place empty - ' '
			mEventsList.push_back(PacEvent(ScoreChange, (ghostpos + 1) * 2)); // input the score
			checkPills();
			soundGame->eat.play(); // sound of the eating
			break;
		case GreenCookie:
			initialByCookie(tmp, speedGreen, GreenCookie, overallSpeed); // change the speed & the score
			break;
		case BlueCookie:
			initialByCookie(tmp, speedBlue, BlueCookie, overallSpeed);
			break;
		}
		if(moveables[Pac]->entity.nextMove != None &&
			enterable(fetchTileAt(moveables[Pac]->entity.getNode(),moveables[Pac]->entity.getVectorFromNext()))) // if the pac can enter in the next cell
		{
			moveables[Pac]->entity.direction=moveables[Pac]->entity.nextMove;
			moveables[Pac]->entity.nextMove=None;
		}
		if(!enterable(fetchTileAt(moveables[Pac]->entity.getNode(),moveables[Pac]->entity.getVectorFromDirection()))) // the next direction is nove to avoid the next move
			moveables[Pac]->entity.direction=None;

		update=moveables[Pac]->entity.getVectorFromDirection(); // get the next pac's move
	}
}

void PacEngine::checkCollisions()
{
	int doit = false; // avoit if we have a lot of events in the queue
	for(unsigned int i = 1; i <= startPos.size(); ++i)
		if(moveables[Pac]->entity.getNode() == moveables[i]->entity.getNode())
				doit = true; // had a Collision

	if(!doit) // finish the function
		return;
	//else decrease the live points
	if(--mLives)
	{
		overallSpeed = initialSpeed; // Initializing the speed 
		mEventsList.push_back(PacEvent(PacDied, mLives)); // input this event
	}
	else
		mEventsList.push_back(PacEvent(GameOver,0)); // input the event - gameover
	resetPositions();
}

void PacEngine::resetPositions()
{
	lastCookieEaten = lastCookie; // red cookie
	if (moveables.size() >= 2)
		for (unsigned int i = 1; i <= startPos.size(); i++)
			moveables[i]->entity.position = startPos[i - 1];

	moveables[Pac]->entity.position = startPacPos;
	moveables[Pac]->entity.direction = None;
	moveables[Pac]->entity.nextMove = None;
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
	int tmp = 0;
	for(int y = 0; y < horizontal; ++y) // conter the cookies in the all map
		for(int x = 0; x < vertical; ++x)
			if(gameObjects[x][y]->symbol == RedCookie ||gameObjects[x][y]->symbol == BlueCookie ||gameObjects[x][y]->symbol == GreenCookie)
				++tmp;

	if(tmp == 0) // the current level had been finished
		mEventsList.push_back(PacEvent(PillsExhausted, 0));
}

void PacEngine::updateGhost(int who)
{
	float currentTime = globalClock.getElapsedTime().asSeconds();
	if (currentTime - lastBFSupdate > 0.5f || lastBFSupdate == 0)
	{
		updateBFS(moveables[Pac]->entity.getNode());
		lastBFSupdate = currentTime;
	}

	if(moveables[who]->entity.isAtNode())
	{
		if (moveables[who]->isSmart == true)
		{
			//chose next move:
			moveables[who]->entity.target = getTarg(who);
			moveables[who]->entity.direction = getNextMove(moveables[who]->entity);
		}
		else
		{
			int random = rand() % 4;
			moveables[who]->entity.direction = eDirection(random);
			// checks if the direction is good
			if (!enterable(fetchTileAt(moveables[who]->entity.getNode(), moveables[who]->entity.getVectorFromDirection())))
				moveables[who]->entity.direction = None;
		}
	}

	sf::Vector2i update = moveables[who]->entity.getVectorFromDirection();
	for (int i = 0; i < moveables[who]->entity.speed; ++i) {
		moveables[who]->entity.position += update;
		if (moveables[who]->entity.isAtNode())
			break;
	}
}

sf::Vector2i PacEngine::getTarg(int who)
{
	return moveables[Pac]->entity.getNode();
}

void PacEngine::initSpeed()
{
	moveables[Pac]->entity.speed = initialSpeed;
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
	if (lastCookieEaten == GreenCookie) // if is green or blue
		overallSpeed += (overallSpeed / speed);
	else
		overallSpeed -= (overallSpeed / speed);
}

eDirection PacEngine::getNextMove(PacEntity& ent)
{
	std::vector<sf::Vector2i> possibleNodes;
	sf::Vector2i tmp = ent.getNode();
	
	if (bfsMap[tmp.y][tmp.x].pi.x == -1)
		return None;
	auto a = bfsMap[tmp.y][tmp.x].pi - tmp;
	return getDirFromVec(a);


	// checks if the move is possible 
	/*if(enterable(fetchTileAt(ent.getNode(),sf::Vector2i(PlusOne,0))))
		possibleNodes.push_back(ent.getNode()+sf::Vector2i(PlusOne,0));
	if(enterable(fetchTileAt(ent.getNode(),sf::Vector2i(MinusOne,0))))
		possibleNodes.push_back(ent.getNode()+sf::Vector2i(MinusOne,0));
	if(enterable(fetchTileAt(ent.getNode(),sf::Vector2i(0,PlusOne))))
		possibleNodes.push_back(ent.getNode()+sf::Vector2i(0,PlusOne));
	if(enterable(fetchTileAt(ent.getNode(),sf::Vector2i(0, MinusOne))))
		possibleNodes.push_back(ent.getNode()+sf::Vector2i(0, MinusOne));

	for(std::vector<sf::Vector2i>::iterator it = possibleNodes.begin(); it != possibleNodes.end(); ++it)
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
		if(a > get2DistanceFromNodes(ent.target, possibleNodes[i]))
		{
			a = get2DistanceFromNodes(ent.target,possibleNodes[i]);
			b = i;
		}
	}

	if(b != -1)
		return getDirFromVec(possibleNodes[b] - ent.getNode());
	else
		return getDirFromVec(b * ent.getVectorFromDirection());*/
}