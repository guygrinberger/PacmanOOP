#include "Object.h"

Object::Object(ITEM symbol) : index(sf::Vector2i(0, 0)), _symbol(symbol)
{
}

Object::~Object()
{
}

ITEM Object::getSymbol() {
	return _symbol;
}

ITEM Object::getColor() {
	return _color;
}

void Object::setSymbol(ITEM symbol) {
	_symbol = symbol;
}

void Object::setColor(ITEM color) {
	_color = color;
}