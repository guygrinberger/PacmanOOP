#include "MenuState.h"
#include "StateManager.h"
#include <SFML/Window/Event.hpp>
#include "ScoreState.h"
#include "GameState.h"
#include <iostream>

MenuState::MenuState(float width, float height){
	font.loadFromFile("Endless Wall.ttf");
	BG.loadFromFile("pacmoon.jpg", sf::IntRect(0, 0, pic, pic));
	background.setColor(sf::Color(255, 255, 255, 128));
	background.setTexture(BG);
	background.setScale((float)width / pic, (float)height / pic);

	menu[PLAY_PLACE].setFont(font);
	menu[PLAY_PLACE].setFillColor(sf::Color::Green);
	menu[PLAY_PLACE].setString("Play");
	menu[PLAY_PLACE].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	menu[MAP_MAKER].setFont(font);
	menu[MAP_MAKER].setFillColor(sf::Color::White);
	menu[MAP_MAKER].setString("MapMaker");
	menu[MAP_MAKER].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	menu[SCORE_BOARD].setFont(font);
	menu[SCORE_BOARD].setFillColor(sf::Color::White);
	menu[SCORE_BOARD].setString("ScoreBoard");
	menu[SCORE_BOARD].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

	menu[EXIT].setFont(font);
	menu[EXIT].setFillColor(sf::Color::White);
	menu[EXIT].setString("Exit");
	menu[EXIT].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 4));
	selectedItemIndex = PLAY_PLACE;

	menu[MUTE].setFont(font);
	menu[MUTE].setFillColor(sf::Color::White);
	menu[MUTE].setString("Mute");
	menu[MUTE].setPosition(sf::Vector2f(0, 0));
}

void MenuState::run(PointerPack& pack)
{
	for(int i =0 ; i <MAX_NUMBER_OF_ITEMS; i++)
		menu[i].setFont(*pack.Font);
	Menu = std::make_unique<MenuState>((float)650, (float)650);
	while (pack.Window->isOpen())
	{
		sf::Event event;
		while (pack.Window->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				pack.Manager->popTop(100);
				pack.Window->close();
				return;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					Menu->MoveUp();
					pack.Sound->buttom.play();
					break;
				case sf::Keyboard::Down:
					Menu->MoveDown();
					pack.Sound->buttom.play();
					break;
				case sf::Keyboard::Return: // enter key
					pack.Sound->welcome.play();
					makeChoose(pack);
					if (Menu->saveChoose >= PLAY_PLACE && Menu->saveChoose <= EXIT)
						return;
					break;
				}
			case sf::Event::MouseButtonReleased:
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					Menu->getSelection(pack.Window->getPosition().x, pack.Window->getPosition().y, sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
					if (Menu->saveChoose >= PLAY_PLACE && Menu->saveChoose <= MUTE)
					{
						pack.Sound->welcome.play();
						makeChoose(pack);
						if (Menu->saveChoose >= PLAY_PLACE && Menu->saveChoose <= EXIT)
							return;
					}
					break;
				}
			case sf::Event::MouseMoved:
				if (!(Menu->getSelection(pack.Window->getPosition().x, pack.Window->getPosition().y, sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)))
					pack.Sound->buttom.play();
				break;
			}
			break;
		}
		pack.Window->clear();
		pack.Window->draw(background);
		for (int i = 0; i <MAX_NUMBER_OF_ITEMS; i++)
			pack.Window->draw(Menu->menu[i]);
		pack.Window->display();
	}
}
void MenuState::makeChoose(PointerPack & pack)
{
	switch (Menu->getPlace())
	{
	case PLAY_PLACE:
	{
		pack.Manager->pushTop(new GameState());
		break;
	}
	case MAP_MAKER:
		if (std::ifstream("level1.txt")) {
			std::ifstream file("level1.txt"); //hold path
			std::string line; //string for every line
			getline(file, line);
			sizeRow = atoi(line.c_str());
			getline(file, line);
			sizeCol = atoi(line.c_str());
			Painter{ "Map/icons.jpg", "level1.txt" , sizeRow, sizeCol }.run();// Start running the painter
		}
		else {
			cout << "input the size of row of the map: " << endl;
			cin >> sizeRow;
			cout << "input the size of col of the map: " << endl;
			cin >> sizeCol;
			Painter{ "Map/icons.jpg", "level1.txt" , sizeRow, sizeCol }.run();// Start running the painter
		}
		break;

	case SCORE_BOARD:
	{
		pack.Manager->pushTop(new ScoreState(-1));
		break;
	}
	case EXIT:
		pack.Sound->bye.play();
		pack.Manager->popTop(100);
		pack.Window->close();
		break;
	case MUTE:
		if (!pack.Sound->muted)
		{
			pack.Sound->Background.pause();
			pack.Sound->muted = true;
		}
		else
		{
			pack.Sound->Background.play();
			pack.Sound->Background.setLoop(true);
			pack.Sound->muted = false;
		}
		break;
	}
}
void MenuState::draw(sf::RenderWindow & window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
		window.draw(menu[i]);
}

void MenuState::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Green);
	}
	saveChoose = selectedItemIndex;
}

void MenuState::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Green);
	}
	saveChoose = selectedItemIndex;
}

bool MenuState::getSelection(int indexXwindow, int indexYwindow, int indexXmouse, int indexYmouse)
{
	whatSelected = checkPlace(indexXwindow, indexYwindow, indexXmouse, indexYmouse);
	if (whatSelected == saveChoose || whatSelected == FAIL)
		return true;
	else
		samePlace = false;
	switch (whatSelected)
	{
	case PLAY_PLACE:
		moveIt(saveChoose, whatSelected, selectedItemIndex);
		break;
	case MAP_MAKER:
		moveIt(saveChoose, whatSelected, selectedItemIndex);
		break;
	case SCORE_BOARD:
		moveIt(saveChoose, whatSelected, selectedItemIndex);
		break;
	case EXIT:
		moveIt(saveChoose, whatSelected, selectedItemIndex);
		break;
	case MUTE:
		moveIt(saveChoose, whatSelected, selectedItemIndex);
		break;
	}
	saveChoose = selectedItemIndex;
	return samePlace;
}

int MenuState::getPlace()
{
	return saveChoose;
}

void MenuState::moveIt(int saveChoose, int whatSelected, int & selectedItemIndex)
{
	if (saveChoose > whatSelected || saveChoose < whatSelected)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex = whatSelected;
		menu[selectedItemIndex].setFillColor(sf::Color::Green);
	}
}

places MenuState::checkPlace(int indexXwindow, int indexYwindow, int indexXmouse, int indexYmouse)
{
	if (indexXmouse - indexXwindow > menu[PLAY_PLACE].getPosition().x)
	{
		if (indexXmouse - indexXwindow < 390 && indexYmouse - indexYwindow >(menu[PLAY_PLACE].getPosition().y + ADDLOW) && indexYmouse - indexYwindow < (menu[PLAY_PLACE].getPosition().y + ADDHIGH))
			return PLAY_PLACE;
		if (indexXmouse - indexXwindow < 470 && indexYmouse - indexYwindow >(menu[MAP_MAKER].getPosition().y + ADDLOW) && indexYmouse - indexYwindow < (menu[MAP_MAKER].getPosition().y + ADDHIGH))
			return MAP_MAKER;
		if (indexXmouse - indexXwindow < 500 && indexYmouse - indexYwindow >(menu[SCORE_BOARD].getPosition().y + ADDLOW) && indexYmouse - indexYwindow < (menu[SCORE_BOARD].getPosition().y + ADDHIGH))
			return SCORE_BOARD;
		if (indexXmouse - indexXwindow < 380 && indexYmouse - indexYwindow >(menu[EXIT].getPosition().y + ADDLOW) && indexYmouse - indexYwindow < (menu[EXIT].getPosition().y + ADDHIGH))
			return EXIT;
	}
	if (indexXmouse - indexXwindow >= 0 && indexXmouse - indexXwindow < 70 && indexYmouse - indexYwindow >= 0 && indexYmouse - indexYwindow < 70)
		return MUTE;
	return FAIL;
}
