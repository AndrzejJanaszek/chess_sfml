#pragma once
#include "Constants.h"
#include "Board.h"
#include <iostream>

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
	Move getMove(Board& board);

	double evalPosition(Board &board);

	//bes move for deptn = n
	Move depthSearch(Board& board, int depth);

};