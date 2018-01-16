#include "Painter.h"
#include <vector>
using std::vector;
using std::cin;
using std::cout;
using std::endl;

Painter::Painter(std::string iconsFile, std::string mapFile, const unsigned int mapSizeRow, const unsigned int mapSizeCol)
	:mapFileName(mapFile), menu(iconsTexture), _mapSizeRow(mapSizeRow), _mapSizeCol(mapSizeCol) , window(sf::VideoMode(650, 650), "Map Editior", sf::Style::Default)
{
	iconsTexture.loadFromFile(iconsFile);
	menu.setMenuSprite();
	if (_mapSizeRow >= _mapSizeCol)
		iconSize = windowSize.y - menu.getMenuHeight() - _mapSizeRow;// Calculate the gird size
	else
		iconSize = windowSize.y - menu.getMenuHeight() - _mapSizeCol;// Calculate the gird size
	horizontalLocation = (windowSize.x - iconSize) / 2;	// get the horizintal location of the matrix
	verticalLocation = (windowSize.y - iconSize - menu.getMenuHeight()) / 2;// get the vertical location of the matrix
	music.openFromFile("Map/place.ogg");
}
Painter::~Painter()
{
}

void Painter::run()
{
	sf::Sprite sprite(iconsTexture);
	buildVector();
	sprite.scale(sf::Vector2f(cellSize() / (float)64, cellSize() / (float)64));// make size of the sprite

	if (std::ifstream(mapFileName))	
		getText();
	while (window.isOpen()) // Keep the window open
	{
		window.clear();	//Clear the window
		mousePos = sf::Mouse::getPosition(window);	// Get the mouse position in reation to the window
		objectSize = getIndexIcon(mousePos, _mapSizeRow, _mapSizeCol);
		drawMatrix(sprite);// Draw
		setLines(); //paint the lines
		menu.drawMenu(window); // draw again the menu
		window.display();
		
		if (window.waitEvent(event)) {// Start listening to events
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed: 
			{
				clicked = true;
				if (isValid(objectSize) && clicked)
				{
					if (gameObjects[objectSize.y][objectSize.x] != nullptr) // when the object pointer on something
						objectNotNull();
					if (gameObjects[objectSize.y][objectSize.x] == nullptr)  // when the object not pointer on something
					{
						switch (menu.getSelected()) 
						{
						case PACMAN:
							if (!havePacman) {
								gameObjects[objectSize.y][objectSize.x] = new Pacman;
								havePacman = true;
							}
							else
								changePacmanPosition();
							music.play();
							break;
						case ENEMY:
							gameObjects[objectSize.y][objectSize.x] = new Enemy;
							music.play();
							break;
						case COOKIE:
							gameObjects[objectSize.y][objectSize.x] = new Cookie;
							music.play();
							break;
						case WALL:
							gameObjects[objectSize.y][objectSize.x] = new Wall;
							music.play();
							break;
						case RED:
						case GREEN:
						case BLUE:
							color = menu.getSelected();
						}
						if(gameObjects[objectSize.y][objectSize.x] != nullptr)
							gameObjects[objectSize.y][objectSize.x]->setColor(color);
					}
				}
				break;
			}
			case sf::Event::MouseButtonReleased:
				clicked = false;
				if (event.mouseButton.button == sf::Mouse::Left)// Only left mouse button can change the menu
					menu.selectItem(mousePos);// Select an menu item
				switch (menu.getSelected())
				{
				case RED:
				case GREEN:
				case BLUE:
					color = menu.getSelected();
					break;
				case SAVE:
					if (gameObjects.empty())
						break;
					else
						save();
					break;
				case CLEAR:
					gameObjects.clear();
					buildVector();
					havePacman = false;
					window.close();
					break;
				}
				if (isValid(objectSize))
				{
					if (gameObjects[objectSize.y][objectSize.x] != nullptr && !havePacman)
					{
						if (gameObjects[objectSize.y][objectSize.x]->getSymbol() == PACMAN)
							havePacman = false;
						else
							overWrite(objectSize);	// over-write on the exist object
					}
				}	
				menu.highlight(mousePos);// Hightlight the current menu item
				break;
			}
		}
	}
}

float Painter::cellSize() const{ // get the icon size
	if(_mapSizeRow > _mapSizeCol)
		return ((float)iconSize /_mapSizeRow);
	return ((float)iconSize / _mapSizeCol);
}

ITEM Painter::whichColor(){ // get color
	return ITEM();
}

char Painter::image2char(ITEM character, ITEM color)
{
	switch (character)
	{
	case PACMAN:
		switch (color)
		{
		case RED:
			return '@';
			break;
		case GREEN:
			return 'W';
			break;
		case BLUE:
			return 'S';
			break;
		}
	case WALL:
		switch (color)
		{
		case RED:
			return '#';
			break;
		case GREEN:
			return 'E';
			break;
		case BLUE:
			return 'D';
			break;
		}
	case ENEMY:
		switch (color)
		{
		case RED:
			return '%';
			break;
		case GREEN:
			return 'T';
			break;
		case BLUE:
			return 'G';
			break;
		}
	case COOKIE:
		switch (color)
		{
		case RED:
			return '*';
			break;
		case GREEN:
			return 'I';
			break;
		case BLUE:
			return 'K';
			break;
		}
	default:
		break;
	}
	return 0;
}

void Painter::char2image(char txtChar, vector<std::vector<Object*>>& gameObjects, int i, int j)
{
	switch (txtChar)
	{
	case '@':
		gameObjects[j][i] = new Pacman, gameObjects[j][i]->setColor(RED);
		break;
	case '%':
		gameObjects[j][i] = new Enemy, gameObjects[j][i]->setColor(RED);
		break;
	case '*':
		gameObjects[j][i] = new Cookie, gameObjects[j][i]->setColor(RED);
		break;
	case '#':
		gameObjects[j][i] = new Wall, gameObjects[j][i]->setColor(RED);
		break;
	case 'W':
		gameObjects[j][i] = new Pacman, gameObjects[j][i]->setColor(GREEN);
		break;
	case 'T':
		gameObjects[j][i] = new Enemy, gameObjects[j][i]->setColor(GREEN);
		break;
	case 'I':
		gameObjects[j][i] = new Cookie, gameObjects[j][i]->setColor(GREEN);
		break;
	case 'E':
		gameObjects[j][i] = new Wall, gameObjects[j][i]->setColor(GREEN);
		break;
	case 'S':
		gameObjects[j][i] = new Pacman, gameObjects[j][i]->setColor(BLUE);
		break;
	case 'G':
		gameObjects[j][i] = new Enemy, gameObjects[j][i]->setColor(BLUE);
		break;
	case 'K':
		gameObjects[j][i] = new Cookie, gameObjects[j][i]->setColor(BLUE);
		break;
	case 'D':
		gameObjects[j][i] = new Wall, gameObjects[j][i]->setColor(BLUE);
		break;
	}
}

void Painter::drawMatrix(sf::Sprite sprite)
{
	for (unsigned int i = 0; i <_mapSizeCol; i++) {
		for (unsigned int j = 0; j < _mapSizeRow; j++) {
			if (gameObjects[i][j] != nullptr) {
				sprite.setPosition(getCellPosition(sf::Vector2i(i, j)));
				gameObjects[i][j]->draw(window, sprite);
			}
		}
	}
}

void Painter::deleteIt()
{
	delete gameObjects[objectSize.y][objectSize.x];
	gameObjects[objectSize.y][objectSize.x] = nullptr;
}

void Painter::buildVector()
{
	gameObjects.resize(_mapSizeCol);// make the windows like the map size
	for (unsigned i = 0; i < gameObjects.size(); i++)
		gameObjects[i] = std::vector<Object*>(_mapSizeRow);
}

void Painter::objectNotNull()
{
	if (!havePacman && menu.getSelected() != RED && menu.getSelected() != GREEN && menu.getSelected() != BLUE)
		deleteIt();
	else if (menu.getSelected() != PACMAN && menu.getSelected() != RED && menu.getSelected() != GREEN && menu.getSelected() != BLUE)
	{
		if (gameObjects[objectSize.y][objectSize.x]->getSymbol() == PACMAN)
			havePacman = false;
		deleteIt();
	}
}

void Painter::changePacmanPosition()
{
	for (unsigned int i = 0; i < _mapSizeCol; i++) {
		for (unsigned int j = 0; j < _mapSizeRow; j++) {
			if (gameObjects[i][j] != nullptr && gameObjects[i][j]->getSymbol() == PACMAN) {
				gameObjects[i][j] = nullptr;
			}
		}
	}
	gameObjects[objectSize.x][objectSize.y] = new Pacman;
}

void Painter::getText()
{
	std::ifstream file(mapFileName);
	getline(file, line);//get line 
	getline(file, line);//get line 
	for (unsigned int i = 0; i < _mapSizeRow; i++)
	{
		getline(file, line);//get line 
		for (unsigned int j = 0; j < _mapSizeCol; j++) //run on lines
		{
			if (line[j] != '1')
				char2image(line[j], gameObjects, i, j);
			if (line[j] != '1' && gameObjects[j][i]->getSymbol() == PACMAN)
				havePacman = true;
		}
	}
}

void Painter::save()
{
	int index = 0;
	std::ifstream file;
	do 

	{
		index++;
		std::string  level = "level" + std::to_string(index) + ".txt";
		file.open(level);
	} while (file.good());


	std::ofstream output_file("./level" + std::to_string(index) + ".txt");
	for (unsigned int i = 0; i < _mapSizeRow; i++) {
		if (i == 0)
		{
			output_file << _mapSizeRow;
			output_file << "\n";
			output_file <<_mapSizeCol;
		}
		output_file << "\n";
		for (unsigned int j = 0; j < _mapSizeCol; j++) {
			if (gameObjects[j][i] != nullptr)
				output_file << image2char(gameObjects[j][i]->getSymbol(), gameObjects[j][i]->getColor());
			else
				output_file << "1";
		}
	}
}

int Painter::setBiggerSize()
{
	if (_mapSizeRow >= _mapSizeCol)
		return _mapSizeRow;
	else
		return _mapSizeCol;
}

void Painter::setSizeLines()
{
	if (_mapSizeRow > _mapSizeCol)
	{
		row.setSize(sf::Vector2f((float)((float)iconSize / _mapSizeRow * _mapSizeCol), 1.0f));
		col.setSize(sf::Vector2f(1.0f, (float)(iconSize)));
	}
	else if (_mapSizeRow < _mapSizeCol)
	{
		row.setSize(sf::Vector2f((float)(iconSize), 1.0f));
		col.setSize(sf::Vector2f(1.0f, (float)((float)iconSize / _mapSizeCol * _mapSizeRow)));
	}
	else if (_mapSizeRow == _mapSizeCol)
	{
		row.setSize(sf::Vector2f((float)(iconSize), 1.0f));
		col.setSize(sf::Vector2f(1.0f, (float)(iconSize)));
	}

}

sf::Vector2i Painter::makeCoordination(int gridX, int gridY, int size)
{
	int y = static_cast<int>((floor((float)gridX / ((float)iconSize / (float)size)))),
		x = static_cast<int>((floor((float)gridY / ((float)iconSize / (float)size))));
	if (y == size) //if the click is on the edge
		y = y - 1;
	if (x == size)//if the click is on the edge
		x = x - 1;
	if (static_cast<unsigned int>(indexs.x) > _mapSizeRow || static_cast<unsigned int>(indexs.y) > _mapSizeCol) // edges 
		return { -1,-1 };
	return {x,y};
}

bool Painter::isValid(sf::Vector2i gridIndex) {
	if (gridIndex.x != -1 && gridIndex.y != -1)
		return true;
	return false;
}

void Painter::overWrite(sf::Vector2i gridIndex) {
	sf::RectangleShape erase(sf::Vector2f((float)(cellSize()), (float)(cellSize())));
	erase.setFillColor(sf::Color::Yellow);
	erase.setPosition(getCellPosition(gridIndex));
	window.draw(erase);
}

sf::Vector2f Painter::getCellPosition(sf::Vector2i gridIndex) {
	return (sf::Vector2f((horizontalLocation + (gridIndex.x * cellSize())), (verticalLocation + menu.getMenuHeight() + (gridIndex.y * cellSize()))));
}

void Painter::setLines()
{
	int size = setBiggerSize();
	for (int index = 0; index <= size; index++)
	{
		setSizeLines();
		// calculate row position
		Line_x = (float)horizontalLocation;// Calculate row position
		Line_y = menu.getMenuHeight() + verticalLocation + (index * ((float)(iconSize) / size));// Rows y starts from menu height + top margin
		row.setPosition(sf::Vector2f(Line_x, Line_y));
		// calculate col position
		Line_x = horizontalLocation + (index * ((float)iconSize / size));// Columns x starts from side margin
		Line_y = (float)menu.getMenuHeight() + verticalLocation;// Columns y starts from from menu height + top margin
		col.setPosition(sf::Vector2f(Line_x, Line_y));
		if (static_cast<unsigned int>(index) > _mapSizeCol)
			window.draw(row);
		if (static_cast<unsigned int>(index) > _mapSizeRow)
			window.draw(col);
		if (static_cast<unsigned int>(index) <= _mapSizeRow && static_cast<unsigned int>(index) <= _mapSizeCol)
		{
			window.draw(row);
			window.draw(col);
		}
	}	
}

sf::Vector2i Painter::getIndexIcon(sf::Vector2i mousePos, unsigned int mapSizeRow, unsigned int mapSizeCol)
{
	// Calculate icons' coordinates
	int gridX = mousePos.x - horizontalLocation,
		gridY = mousePos.y - menu.getMenuHeight() - verticalLocation;
	if (mapSizeRow >= mapSizeCol)
	{
		if (gridX >= 0 && gridX <= (int)(((float)iconSize / mapSizeRow)*mapSizeCol) && gridY >= 0 && gridY <= iconSize)
		{
			// make int coordinates 
			indexs = makeCoordination(gridX, gridY, _mapSizeRow);
			return sf::Vector2i(indexs.x, indexs.y);
		}
	}
	else
	{
		if (gridX >= 0 && gridX <= iconSize && gridY >= 0 && gridY <= (int)(((float)iconSize/ mapSizeCol)* mapSizeRow))
		{
			// make int coordinates 
			indexs = makeCoordination(gridX, gridY, _mapSizeCol);
			return sf::Vector2i(indexs.x, indexs.y);
		}
	}
	return sf::Vector2i(-1, -1);
}




