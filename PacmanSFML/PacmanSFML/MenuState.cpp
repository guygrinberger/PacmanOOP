#include "MenuState.h"
#include "StateManager.h"
#include <SFML/Window/Event.hpp>
#include "ScoreState.h"
#include "GameState.h"

MenuState::MenuState(){}

void MenuState::run(PointerPack& pack)
{
	sf::Vector2u windowSize = pack.Window->getSize();
	mAtlas.loadFromFile("bibreman2.png");
	yoram.setTexture(mAtlas);
	float yoramY = 600.f;
	yoram.setPosition(10.f, yoramY);
	sf::SoundBuffer introSong, shalomB;
	introSong.loadFromFile("intro.ogg");
	sf::Sound intro, shalom;
	intro.setBuffer(introSong);
	intro.setVolume(20);
	//intro.play();
	shalomB.loadFromFile("shalom.ogg");
	shalom.setBuffer(shalomB);
	//mTitle.setFont(*pack.Font);
	//mPlay.setFont(*pack.Font);
	//mHigh.setFont(*pack.Font);
	//mMapMaker.setFont(*pack.Font);
	//mQuit.setFont(*pack.Font);

	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{

		menu[i].setFont(*pack.Font);
		menu[i].setFillColor(sf::Color::White);
		menu[i].setPosition(sf::Vector2f(windowSize.x / 2 - 50, windowSize.y / (MAX_NUMBER_OF_ITEMS + 1) * (i + 1)));
	}

	menu[0].setFillColor(sf::Color::Green);

	mTitle.setString("Pacman Biberman");
	mPlay.setString("Press P to play");
	menu[PLAY_PLACE].setString("play");
	mHigh.setString("Press H to open highscors");
	menu[SCORE_BOARD].setString("highscors");
	mMapMaker.setString("Press M to open mapMaker");
	menu[MAP_MAKER].setString("mapMaker");
	mQuit.setString("Press Q to quit");
	menu[EXIT].setString("quit");

	selectedItemIndex = PLAY_PLACE;

	//mTitle.setPosition(0.f,75.f);
	//mPlay.setPosition(150.f,200.f);
	//mHigh.setPosition(150.f,235.f);
	//mMapMaker.setPosition(150.f, 270.f);
	//mQuit.setPosition(150.f,305.f);
	sf::Event eve;
	sf::Clock biberclock;
	while(true)
	{
		sf::Event event;
		while(pack.Window->pollEvent(eve))
		{
			////switch (event.type)
			////{
			////case sf::Event::Closed:
			////	pack.Manager->popTop(100);
			////	break;
			////	/*
			////	case::sf::Event::Resized:
			////	sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			////	window.setView(sf::View(visibleArea));
			////	break;*/
			////case sf::Event::KeyPressed:
			////	switch (event.key.code)
			////	{
			////	case sf::Keyboard::Up:
			////		MoveUp();
			////		//sound->buttom.play();
			////		//buttom.play();
			////		break;

			////	case sf::Keyboard::Down:
			////		MoveDown();
			////		//sound->buttom.play();
			////		//buttom.play();
			////		break;
			////	case sf::Keyboard::Return: // enter key
			////		//sound->welcome.play();
			////		//welcome.play();
			////		//makeChoose();
			////		break;
			////	}
			//////case sf::Event::MouseButtonReleased:
			////	switch (event.mouseButton.button)
			////	{
			////	case sf::Mouse::Left:
			////		getSelection(pack.Window.getPosition().x, pack.Window.getPosition().y, sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
			////		if (menu->saveChoose >= PLAY_PLACE && menu->saveChoose <= MUTE)
			////		{
			////			sound->welcome.play();
			////			//welcome.play();
			////			makeChoose();
			////		}
			////		break;
			////	}
			////case sf::Event::MouseMoved:
			////	if (!(getSelection(pack.Window.getPosition().x, pack.Window.getPosition().y, sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)))
			////		sound->buttom.play();
			////	//buttom.play();
			////	break;
			//}
			if(eve.type==sf::Event::Closed||(eve.type==sf::Event::KeyPressed && eve.key.code==sf::Keyboard::Q))
			{ 
				pack.Manager->popTop(100);
				return;
			}
			else if(eve.type == sf::Event::KeyPressed)
			{
				if(eve.key.code==sf::Keyboard::P)
				{
					pack.Manager->pushTop(new GameState());
					return;
				}
				if(eve.key.code == sf::Keyboard::H)
				{
					pack.Manager->pushTop(new ScoreState(-1));
					return;
				}
			}
		}
		pack.Window->clear();
		for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
			pack.Window->draw(menu[i]);
		if(yoramY != 235.f)
		{
			yoram.setPosition(10.f, yoramY);
			yoramY--;
		}
		else
		{

			if (int(biberclock.getElapsedTime().asSeconds()) % 2 == 0)
			{
				mAtlas.loadFromFile("bibreman.png");
				yoram.setTexture(mAtlas);
			}
			else
			{
				mAtlas.loadFromFile("bibreman2.png");
				yoram.setTexture(mAtlas);
				//shalom.play();
			}
		}
		pack.Window->display();
	}
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
	if (whatSelected == saveChoose || whatSelected == -1)
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

