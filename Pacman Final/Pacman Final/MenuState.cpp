#include "MenuState.h"
#include "StateManager.h"
#include <SFML/Window/Event.hpp>
#include "ScoreState.h"
#include "GameState.h"
#include <iostream>

MenuState::MenuState(float width, float height) {
	font.loadFromFile("diablo_h.ttf"); // font
	BG.loadFromFile("pacmoon1.jpg", sf::IntRect(0, 0, pic, pic)); // background pic
	geraT.loadFromFile("gera.png");
	guyT.loadFromFile("guy.png");
	background.setColor(sf::Color(255, 255, 255, 128));// make the pic transparent
	background.setTexture(BG);
	gera.setTexture(geraT);
	guy.setTexture(guyT);
	background.setScale((float)width / pic, (float)height / pic);// set the pic by the windows' size
	gera.setScale(width / 2000.f, height / 2000.f);
	guy.setScale(width / 2666.6666f, height / 2666.6666f);
	gera.setPosition(geraX, 100.f);
	guy.setPosition(guyX, 100.f);

	setMenuItems(PLAY_PLACE, sf::Color::Green, "Play", 1, width, height);
	setMenuItems(MAP_MAKER, sf::Color::White, "MapMaker", 2, width, height);
	setMenuItems(SCORE_BOARD, sf::Color::White, "ScoreBoard", 3, width, height);
	setMenuItems(EXIT, sf::Color::White, "Exit", 4, width, height);
	setMenuItems(MUTE, sf::Color::White, "Mute", 0, 0, 0);
	selectedItemIndex = PLAY_PLACE;
}

void MenuState::setMenuItems(int index, sf::Color color, string text, int position, float width, float height) {
	menu[index].setFont(font);
	menu[index].setFillColor(color);
	menu[index].setString(text);
	menu[index].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * position));
}

void MenuState::run(PointerPack& pack)
{
	for(int i =0 ; i <MAX_NUMBER_OF_ITEMS; i++)
		menu[i].setFont(*pack.Font); // input for all options the font
	Menu = std::make_unique<MenuState>((float)pic, (float)pic); // Initializing the menu unique pointer
	while (pack.Window->isOpen())
	{
		sf::Event event;
		while (pack.Window->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				pack.Manager->popTop(clean);
				pack.Window->close();
				return;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					Menu->MoveUp(); // 
					pack.Sound->buttom.play(); // makes the sound
					break;
				case sf::Keyboard::Down:
					Menu->MoveDown();
					pack.Sound->buttom.play();
					break;
				case sf::Keyboard::Return: // enter key
					pack.Sound->welcome.play();
					makeChoose(pack);  // checks which button has been clicked
					// if the last choice was between right values - return to make the next action in the stack
					if (Menu->saveChoose >= PLAY_PLACE && Menu->saveChoose <= EXIT) 
						return;
					break; // helps to keep the menu on the screen
				}
			case sf::Event::MouseButtonReleased:
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					// checks the click
					Menu->getSelection(pack.Window->getPosition().x, pack.Window->getPosition().y, sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
					// if the press was on some button
					if (Menu->saveChoose >= PLAY_PLACE && Menu->saveChoose <= MUTE)
					{
						pack.Sound->welcome.play();
						makeChoose(pack);
						// if the last choice was between right values - return to make the next action in the stack
						if (Menu->saveChoose >= PLAY_PLACE && Menu->saveChoose <= EXIT)
							return;
					}
					break;
				}
			case sf::Event::MouseMoved:
				// avoid the click's sound every mouse's move
				if (!(Menu->getSelection(pack.Window->getPosition().x, pack.Window->getPosition().y, sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)))
					pack.Sound->buttom.play();
				break;
			}
			break;
		}
		pack.Window->clear();
		pack.Window->draw(background); // draws the background
		
		justFun(); // for animation of us
		pack.Window->draw(gera); // draw us
		pack.Window->draw(guy);
		for (int i = 0; i <MAX_NUMBER_OF_ITEMS; i++) // draw the menu
			pack.Window->draw(Menu->menu[i]);
		pack.Window->display();
	}
}
void MenuState::makeChoose(PointerPack & pack)
{
	switch (Menu->getPlace()) // make the option by the right enum
	{
	case PLAY_PLACE:
	{
		pack.Manager->pushTop(new GameState()); // open the game
		break;
	}
	case MAP_MAKER:
	{
		std::ifstream file;
		std::string level = "Level1.txt";
		numOfFiles = 1;
		file.open(level);
		while (file.good()) { // checks if the file is ok, to know which map to create
			file.close();
			numOfFiles++;
			level = ("Level" + std::to_string(numOfFiles) + ".txt");
			file.open(level);
		}
		cout << "input the size of row of the map: " << endl;
		cin >> sizeRow;
		cout << "input the size of col of the map: " << endl;
		cin >> sizeCol;
		Painter{ "Map/icons.jpg", "Level" + std::to_string(numOfFiles) + ".txt" , sizeRow, sizeCol }.run();// Start running the painter
		break;
	}
	
	case SCORE_BOARD:
	{
		pack.Manager->pushTop(new ScoreState(nothing)); // open the scoreboard
		break;
	}
	case EXIT:
		pack.Sound->bye.play(); // makes sound
		pack.Manager->popTop(clean); // clear
		pack.Window->close(); // close the windows
		break;
	case MUTE:
		if (!pack.Sound->muted) // if false - the button never has been clicked
		{
			pack.Sound->Background.pause(); // pause the soundtrack of the game
			pack.Sound->muted = true; // makes true to know the button has been pressed
		}
		else
		{
			pack.Sound->Background.play(); // play the soundtrack again
			pack.Sound->Background.setLoop(true);
			pack.Sound->muted = false;
		}
		break;
	}
}
void MenuState::draw(sf::RenderWindow & window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
		window.draw(menu[i]); // draws the all menu
}

void MenuState::MoveUp()
{
	if (selectedItemIndex - 1 >= 0) // avoid if the counter (number of cell) not exists in the array
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White); // the current button need to be white
		selectedItemIndex--; // decrease in the menu array
		menu[selectedItemIndex].setFillColor(sf::Color::Green);// the chosen button make him green to know the button has been clicked
	}
	saveChoose = selectedItemIndex; // helps to know which option has been chosen
}

void MenuState::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS) // avoid if the counter (number of cell) not exists in the array
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White); // the current button need to be white
		selectedItemIndex++;// increase in the menu array
		menu[selectedItemIndex].setFillColor(sf::Color::Green); // the chosen button make him green to know the button has been clicked
	}
	saveChoose = selectedItemIndex;// helps to know which option has been chosen
}

bool MenuState::getSelection(int indexXwindow, int indexYwindow, int indexXmouse, int indexYmouse)
{
	whatSelected = checkPlace(indexXwindow, indexYwindow, indexXmouse, indexYmouse); // gets the right button 
	if (whatSelected == saveChoose || whatSelected == FAIL) // if the click wasn't on button or on the same button twist return true
	{
		samePlace = true;
		return samePlace;
	}
	else
		samePlace = false;
	switch (whatSelected) // by the right enum - move the green color to the current button
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
	saveChoose = selectedItemIndex; // helps to know which option has been chosen
	return samePlace;
}

int MenuState::getPlace()
{
	return saveChoose;
}

void MenuState::moveIt(int saveChoose, int whatSelected, int & selectedItemIndex)
{
	if (saveChoose > whatSelected || saveChoose < whatSelected) // if the click was on the button
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White); // the current button need to be white
		selectedItemIndex = whatSelected; // change to the right cell in the menu array
		menu[selectedItemIndex].setFillColor(sf::Color::Green);// the chosen button make him green to know the button has been clicked
	}
}

void MenuState::justFun()
{
	if (geraInPlace != true)
	{
		gera.setPosition(geraX, 100.f);
		geraX++;
		if (geraX == 0)
			geraInPlace = true;
	}
	if (geraInPlace == true)
	{
		gera.setPosition(geraX, 100.f);
		geraX--;
		if (geraX == -200)
			geraInPlace = false;
	}

	if (guyInPlace != true)
	{
		guy.setPosition(guyX, 100.f);
		guyX++;
		if (guyX == 720)
			guyInPlace = true;
	}
	if (guyInPlace == true)
	{
		guy.setPosition(guyX, 100.f);
		guyX--;
		if (guyX == 520)
			guyInPlace = false;
	}
}

places MenuState::checkPlace(int indexXwindow, int indexYwindow, int indexXmouse, int indexYmouse)
{
	if (indexXmouse - indexXwindow > menu[PLAY_PLACE].getPosition().x) // checks if the index x starts from some const index
	{
		// checks the index to know if the click has been on some of the buttons
		if (indexXmouse - indexXwindow < 410 && indexYmouse - indexYwindow >(menu[PLAY_PLACE].getPosition().y + ADDLOW) && indexYmouse - indexYwindow < (menu[PLAY_PLACE].getPosition().y + ADDHIGH))
			return PLAY_PLACE;
		if (indexXmouse - indexXwindow < 520 && indexYmouse - indexYwindow >(menu[MAP_MAKER].getPosition().y + ADDLOW) && indexYmouse - indexYwindow < (menu[MAP_MAKER].getPosition().y + ADDHIGH))
			return MAP_MAKER;
		if (indexXmouse - indexXwindow < 535 && indexYmouse - indexYwindow >(menu[SCORE_BOARD].getPosition().y + ADDLOW) && indexYmouse - indexYwindow < (menu[SCORE_BOARD].getPosition().y + ADDHIGH))
			return SCORE_BOARD;
		if (indexXmouse - indexXwindow < 410 && indexYmouse - indexYwindow >(menu[EXIT].getPosition().y + ADDLOW) && indexYmouse - indexYwindow < (menu[EXIT].getPosition().y + ADDHIGH))
			return EXIT;
	}
	// checks if the click has been mute option
	if (indexXmouse - indexXwindow >= 0 && indexXmouse - indexXwindow < 100 && indexYmouse - indexYwindow >= 0 && indexYmouse - indexYwindow < 70) 
		return MUTE;
	return FAIL; // if the click was on the wrong place
}
