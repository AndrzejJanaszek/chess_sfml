#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Piece
{
private:


public:
	Piece(std::string fenType);
	~Piece();

	sf::Sprite sprite;
	std::string type;

	ColorType getColor();
};

