#pragma once
#include "Constants.h"

class BotMachina
{
private:
	ColorType color;

	//future
	unsigned int depth;
public:
	BotMachina();
	~BotMachina();

	Move getMove(std::string positionFEN);
};

