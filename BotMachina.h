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

	//best move in position depth 1
	Move getMove(std::string positionFEN);

	double evalPosition(Board board);

	//bes move for deptn = n
	Move depthSearch(std::string positionFEN, int depth);

};