#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include "CreateMenu.h"
#include "Object.h"
#include "Pacman.h"
#include "Enemy.h"
#include "Cookie.h"
#include "Wall.h"
using std::vector;
using std::string;

class Painter
{
public:
	Painter(std::string iconsFile, std::string mapFile, const unsigned int mapSizeRow, const unsigned int mapSizeCol);// Constructor
	~Painter();// Destructor
	void run();// Start runnig the main window
	float cellSize() const;
	ITEM whichColor(); // checks the color
	char image2char(ITEM character, ITEM color);
	void char2image(char txtChar, vector<std::vector<Object*>> &gameObjects, int i, int j);
	
private:
	unsigned int _mapSizeRow;// Map size
	unsigned int _mapSizeCol;
	int iconSize;// Grid size
	int horizontalLocation;// Grid left and right margin
	int verticalLocation;// Grid top and bottom margin
	bool clicked = false; // if the mouse have been clicked
	bool havePacman = false; // if puted one pacman
	float Line_x; // coordinate of x
	float Line_y; // coordinate of y
	ITEM color = RED; // defult 
	CreateMenu menu;// Painter menu	
	ITEM _color; // contain the color
	vector<std::vector<Object*>> gameObjects;// Pacman game objects	
	string mapFileName;// Game map file name	
	sf::RenderWindow window;// Main window	
	sf::Vector2u windowSize = sf::Vector2u(650, 650);// Window size	
	sf::Texture iconsTexture;// Painter textures
	bool isValid(sf::Vector2i gridIndex);// check if the grid is valid
	void setLines();// Set the lines
	void overWrite(sf::Vector2i gridIndex);// over-write
	sf::Vector2i getIndexIcon(sf::Vector2i mousePos, unsigned int mapSizeRow, unsigned int mapSizeCol);// get x,y indexs of grid 
	sf::Vector2f getCellPosition(sf::Vector2i gridIndex);// Get a grid position
	sf::RectangleShape row;
	sf::RectangleShape col;
	sf::Music music;
	string line;
	sf::Vector2i objectSize;
	sf::Event event;
	sf::Vector2i mousePos;
	void drawMatrix(sf::Sprite sprite);
	void deleteIt();
	void buildVector();
	void objectNotNull();
	void changePacmanPosition();
	void getText();
	void save();
	int setBiggerSize();
	void setSizeLines();
	sf::Vector2i indexs;
	sf::Vector2i makeCoordination(int gridX, int gridY, int size);
	vector<std::vector<Object*>> levels;
};