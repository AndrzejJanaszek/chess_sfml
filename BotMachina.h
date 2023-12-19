#pragma once
#include "Constants.h"
#include "Board.h"

class BotMachina
{
//private:
public:
	ColorType color;

	//future
	unsigned int depth;

	BotMachina();
	BotMachina(ColorType color);
	virtual ~BotMachina();

	Move getMove(std::string positionFEN);

	double evalPosition(Board board);
};