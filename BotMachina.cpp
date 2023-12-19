#include "BotMachina.h"
#include "Board.h"

BotMachina::BotMachina()
{
}

BotMachina::~BotMachina()
{
}

//todo: getMove(unsigned int depth,...)
std::pair<sf::Vector2i, Move> BotMachina::getMove(std::string positionFEN) {
	//expected(positionFEN) [position] [botColor] ...
	Board board(positionFEN);
	std::vector<Move> allPossibleMoves;
	//---

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			//if bot's piece
			if (board.at(row, col) != nullptr && board.at(row, col)->getColor() == color) {
				std::vector<Move> piecePossibleMoves = board.getPossibleMoves(row, col, this->color);
				//combine (insert into all moves) all moves and piece moves
				allPossibleMoves.insert(allPossibleMoves.begin(), piecePossibleMoves.begin(), piecePossibleMoves.end());
			}	
		}
	}

	//---
	return allPossibleMoves.at(rand() % allPossibleMoves.size());
}