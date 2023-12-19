#include "BotMachina.h"
#include "Constants.h"
#include "Piece.h"
#include "Board.h"
#include <iostream>


BotMachina::BotMachina() {
	std::cout << "Def const\n";
}

BotMachina::BotMachina(ColorType color)
{
	this->color = color;
	std::cout << "ColorType color const\n";
}

BotMachina::~BotMachina()
{
	std::cout << "~BotMachina\n";
}

//todo: getMove(unsigned int depth,...)
Move BotMachina::getMove(std::string positionFEN) {
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
	if (allPossibleMoves.size() == 0) {
		Move emptyMove;
		emptyMove.setEmpty();
		return emptyMove;
	}
	return allPossibleMoves.at(rand() % allPossibleMoves.size());
}