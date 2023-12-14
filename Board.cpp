#include "Board.h"
#include <string>
#include <iostream>

Board::Board()
{
	initVariables();

	this->initBoard();
	this->print();
}

Board::~Board()
{
	delete[] board;
}

void Board::initVariables() {
	//this->board = nullptr;
	this->board = new Piece*[8*8];
}

void Board::initBoard() {
	//if empty: -
	//const char empty = '-';
	std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	//std::string fen = "3q4/8/6P1/PK1N1k2/PP1p4/p5np/4pP1p/5r2 w - - 0 1";

	int j = 0;
	for (int i = 0; i < fen.length(); i++) {
		if (j >= 64)
			break;
		//skip /
		if (fen[i] == '/') {
			continue;
		}
		//if digit (<57) will never be 9
		//fill empty
		if (48 <= fen[i] && fen[i] <= 57) {
			for (int ii = 0; ii < fen[i] - 48; ii++) {
				board[j] = nullptr;
				j++;
			}
			continue;
		}

		//piece
		board[j] = new Piece( std::string(1,fen[i]) );
		int row = j / 8;
		int col = j % 8;
		board[j]->sprite.setPosition(100 * col, 100 * row);
		j++;
	}

}

Piece* Board::at(int row, int col) {
	return board[row * 8 + col];
}

void Board::print() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->board[i * 8 + j] != nullptr) {
				std::cout << this->board[i*8+j]->type;
			}
		}
		std::cout << "\n";
	}
	std::cout << "\nKoniec Wypisu\n";
}