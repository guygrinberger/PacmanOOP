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
	gera.setScale(0.4f, 0.4f);
	guy.setScale(0.3f, 0.3f);
	gera.setPosition(geraX, 100.f);
	guy.setPosition(guyX, 100.f);

	menu[PLAY_PLACE].setFont(font); // font
	menu[PLAY_PLACE].setFillColor(sf::Color::Green); // default color
	menu[PLAY_PLACE].setString("Play"); // the string 
	menu[PLAY_PLACE].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1)); // position

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
	for (int i = 0; i <MAX_NUMBER_OF_ITEMS; i++)
		menu[i].setFont(*pack.Font); // input for all options the font
	Menu = std::make_unique<MenuState>((float)board, (float)board); // Initializing the menu unique pointer
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
					isClicked = true;
					// checks the click
					// if the press was on some button
					if (!(Menu->getSelection(float(event.mouseButton.x), float(event.mouseButton.y), isClicked)))
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
				isClicked = false;
				// avoid the click's sound every mouse's move
				if (!(Menu->getSelection(float(event.mouseMove.x), float(event.mouseMove.y), isClicked)))
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

bool MenuState::getSelection(float indexXmouse, float indexYmouse, bool isClicked)
{
	whatSelected = checkPlace(indexXmouse, indexYmouse); // gets the right button 
	if (whatSelected == saveChoose && !isClicked || whatSelected == FAIL && isClicked) // if the click wasn't on button or on the same button twist return true
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

places MenuState::checkPlace(float indexXmouse, float indexYmouse)
{
	// checks the index to know if the click has been on some of the buttons
	if (menu[PLAY_PLACE].getGlobalBounds().contains({ indexXmouse,indexYmouse }))
		return PLAY_PLACE;
	if (menu[MAP_MAKER].getGlobalBounds().contains({ indexXmouse,indexYmouse }))
		return MAP_MAKER;
	if (menu[SCORE_BOARD].getGlobalBounds().contains({ indexXmouse,indexYmouse }))
		return SCORE_BOARD;
	if (menu[EXIT].getGlobalBounds().contains({ indexXmouse,indexYmouse }))
		return EXIT;
	// checks if the click has been mute option
	if (menu[MUTE].getGlobalBounds().contains({ indexXmouse,indexYmouse }))
		return MUTE;
	return FAIL; // if the click was on the wrong place
}