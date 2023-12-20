#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Piece
{
private:


public:
	Piece(std::string fenType);
	~Piece();

	std::string type;

	ColorType getColor();
	std::string getCapitalType();
};

