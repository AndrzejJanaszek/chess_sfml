#pragma once
#include "Constants.h"

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
};