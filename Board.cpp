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
	this->enPassant = sf::Vector2i(-1, -1);
}

void Board::initBoard() {
	//if empty: -
	//const char empty = '-';
	//std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	std::string fen = "1r2kbnr/pp1ppppp/2p2q2/1NP5/B2b4/1n3NQ1/PP1P1PPP/R3KB1R w KQk - 0 1";
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

bool Board::isFriendlyPiece(int row, int col, ColorType activeColor) {
	if (this->isFreeSquare(row,col)) return false;

	if (this->at(row, col)->getColor() == activeColor)
		return true;
	return false;
}

bool Board::isEnemyPiece(int row, int col, ColorType activeColor) {
	if (this->isFreeSquare(row, col)) return false;

	if (this->at(row, col)->getColor() != activeColor)
		return true;
	return false;
}

bool Board::isFreeSquare(int row, int col) {
	if (this->at(row, col) == nullptr) return true;

	return false;
}

std::vector<sf::Vector2i> Board::getPossibleMoves(int row, int col, ColorType activeColor) {
	std::vector<sf::Vector2i> possibleMoves;
	if (this->at(row, col) == nullptr) return possibleMoves;

	std::string pieceType = this->at(row, col)->type;
	if (pieceType[0] >= 97) pieceType = pieceType[0] - 32;
	//pawn

	if (pieceType == "P") {
		//for pawn
		int colorDirectionMultiplier = activeColor == ColorType::LIGHT ? -1 : 1;
		sf::Vector2i attack1 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col - 1);
		sf::Vector2i attack2 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col + 1);
		sf::Vector2i move1 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col);
		sf::Vector2i move2= sf::Vector2i(row + 2 * colorDirectionMultiplier, col);

		//attacks
		//if onBoard AND isEnemy
		if ( this->isMoveOnBoard(attack1.x, attack1.y) ) {
			if ( isEnemyPiece(attack1.x, attack1.y, activeColor) || (enPassant.x == attack1.x && enPassant.y == attack1.y) ) {
				possibleMoves.push_back(attack1);
			}
		}
		if (this->isMoveOnBoard(attack2.x, attack2.y)) {
			if (isEnemyPiece(attack2.x, attack2.y, activeColor) || (enPassant.x == attack2.x && enPassant.y == attack2.y)) {
				possibleMoves.push_back(attack2);
			}
		}

		//move1
		//if onBoard AND isFreeSquare
		if (this->isMoveOnBoard(move1.x, move1.y) && isFreeSquare(move1.x, move1.y)) {
			possibleMoves.push_back(move1);

			//move2
			if (this->isMoveOnBoard(move2.x, move2.y) && isFreeSquare(move2.x, move2.y)) {
				possibleMoves.push_back(move2);
			}
		}
	}

	if (pieceType == "N") {
		std::vector<sf::Vector2i> movesTemp;
		movesTemp.push_back(sf::Vector2i(row + 2, col + 1));
		movesTemp.push_back(sf::Vector2i(row + 2, col - 1));
		movesTemp.push_back(sf::Vector2i(row + 1, col + 2));
		movesTemp.push_back(sf::Vector2i(row + 1, col - 2));
		movesTemp.push_back(sf::Vector2i(row - 1, col + 2));
		movesTemp.push_back(sf::Vector2i(row - 1, col - 2));
		movesTemp.push_back(sf::Vector2i(row - 2, col + 1));
		movesTemp.push_back(sf::Vector2i(row - 2, col - 1));

		for (auto moveT : movesTemp) {
			//if not on board continue
			if (!isMoveOnBoard(moveT.x, moveT.y)) continue;

			if (isFreeSquare(moveT.x, moveT.y) || isEnemyPiece(moveT.x, moveT.y, activeColor) ) {
				possibleMoves.push_back(moveT);
			}
		}
	}

	if (pieceType == "R") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 0));
		moveDirections.push_back(sf::Vector2i(-1, 0));
		moveDirections.push_back(sf::Vector2i(0, 1));
		moveDirections.push_back(sf::Vector2i(0, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					possibleMoves.push_back(move); 
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(move);

					break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "B") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 1));
		moveDirections.push_back(sf::Vector2i(1, -1));
		moveDirections.push_back(sf::Vector2i(-1, 1));
		moveDirections.push_back(sf::Vector2i(-1, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					possibleMoves.push_back(move);
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(move);

					break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "Q") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 0));
		moveDirections.push_back(sf::Vector2i(-1, 0));
		moveDirections.push_back(sf::Vector2i(0, 1));
		moveDirections.push_back(sf::Vector2i(0, -1));
		moveDirections.push_back(sf::Vector2i(1, 1));
		moveDirections.push_back(sf::Vector2i(1, -1));
		moveDirections.push_back(sf::Vector2i(-1, 1));
		moveDirections.push_back(sf::Vector2i(-1, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					possibleMoves.push_back(move);
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(move);

					break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	return possibleMoves;
}

bool Board::isMoveOnBoard(int row, int col) {
	if (0 <= row && row < 8 &&
		0 <= col && col < 8) return true;

	return false;
}

bool Board::isEnPassant() {
	if (this->enPassant.x >= 0 && this->enPassant.y >= 0 && this->enPassant.x < 8 && this->enPassant.y < 8)
		return true;
	return false;
}
sf::Vector2i Board::getEnPassant() {
	return this->enPassant;
}
void Board::setEnPassant(sf::Vector2i newEnPassant) {
	this->enPassant = newEnPassant;
}
void Board::clearEnPassant() {
	this->enPassant = sf::Vector2i(-1,-1);
}