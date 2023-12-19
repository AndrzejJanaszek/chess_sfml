#include "Constants.h"

const std::string PieceName::BLACK_KING = "k";
const std::string PieceName::BLACK_QUEEN = "q";
const std::string PieceName::BLACK_ROOK = "r";
const std::string PieceName::BLACK_KNIGHT = "n";
const std::string PieceName::BLACK_BISHOP = "b";
const std::string PieceName::BLACK_PAWN = "p";
const std::string PieceName::WHITE_KING = "K";
const std::string PieceName::WHITE_QUEEN = "Q";
const std::string PieceName::WHITE_ROOK = "R";
const std::string PieceName::WHITE_KNIGHT = "N";
const std::string PieceName::WHITE_BISHOP = "B";
const std::string PieceName::WHITE_PAWN = "P";
const std::string PieceName::EMPTY = "-";

const std::string TextureName::DARK_SQUARE = "dark_square";
const std::string TextureName::LIGHT_SQUARE = "light_square";
const std::string TextureName::SQUARE_SHADOW = "square_shadow";

Move::Move() {

}
Move::Move(sf::Vector2i from, sf::Vector2i destination, MoveType moveType) {
	this->from = from;
	this->destination = destination;
	this->moveType = moveType;
}
Move::~Move() {
}

void Move::setEmpty() {
	this->moveType = MoveType::EMPTY;
}

bool Move::isEmpty() {
	if (this->moveType == MoveType::EMPTY) return true;

	return false;
}


VALUE_MAP::VALUE_MAP() {}
VALUE_MAP::~VALUE_MAP() {}

//type (piece type) => CAPS : "P" "B" "N" "R" "Q" "K"
double VALUE_MAP::getValueAtPositioin(int row, int col, std::string type, ColorType color) {
	if (color == ColorType::DARK) row = 7 - row;

	int index = 8 * row + col;
	double valuesPAWN[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0.65, 0.75, 0.8, 0.8, 0.75, 0.65, 0, 0, 0, 0.85, 1, 1, 0.85, 0, 0, 0, 0.8, 0.9, 1, 1, 0.9, 0.85, 0, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	double valuesBISHOP[] = { 0.3, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.3, 0.35, 0, 0.45, 0.35, 0.35, 0.45, 0, 0.35, 0, 0.6, 0.55, 0.4, 0.4, 0.55, 0.6, 0, 0.6, 0.75, 0.6, 0.6, 0.6, 0.6, 0.75, 0.6, 0.6, 0.75, 1, 0.65, 0.65, 1, 0.75, 0.6, 0.85, 0.9, 0.75, 1, 1, 0.75, 0.9, 0.85, 0.85, 1, 0.85, 1, 1, 0.85, 1, 0.85, 0.6, 0.75, 0.5, 0, 0, 0.5, 0.75, 0.6 };
	double valuesKNIGHT[] = { 0.2, 0.3, 0.4, 0.4, 0.4, 0.4, 0.3, 0.2, 0.3, 0.5, 0.6, 0.6, 0.6, 0.6, 0.5, 0.3, 0.4, 0.6, 0.65, 0.65, 0.65, 0.65, 0.6, 0.4, 0.4, 0.6, 0.8, 1, 1, 0.8, 0.6, 0.4, 0.45, 0.8, 1, 1, 1, 1, 0.8, 0.45, 0.5, 0.85, 1, 1, 1, 1, 0.85, 0.5, 0.3, 0.6, 0.7, 0.7, 0.7, 0.7, 0.6, 0.3, 0.1, 0.3, 0, 0, 0, 0, 0.3, 0.1 };
	double valuesROOK[] = { 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.4, 0.4, 0.35, 0.35, 0.4, 0.4, 0.6, 0.6, 0.45, 0.35, 0.3, 0.3, 0.35, 0.45, 0.6, 0.6, 0.5, 0.4, 0.3, 0.3, 0.4, 0, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.6, 0.9, 1, 1, 1, 1, 0.9, 0.6 };
	double valuesKING[] = { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.4, 0.5, 0.5, 0.45, 0.2, 0.1, 0.2, 0.4, 0.7, 0.7, 0.7, 0.7, 0.5, 0.2, 0.2, 0, 0.7, 0.7, 0.7, 0.7, 0, 0.2, 0.45, 0.45, 0, 0.6, 0.6, 0, 0.45, 0, 0.7, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.7, 0.7, 0.7, 0.65, 0.4, 0.4, 0.45, 0.7, 0.7, 1, 1, 1, 0.7, 0, 0.7, 1, 1 };

	if (type == "P") {
		return valuesPAWN[index];
	}
	else if (type == "B") {
		return valuesBISHOP[index];
	}
	else if (type == "N") {
		return valuesKNIGHT[index];
	}
	else if (type == "R") {
		return valuesROOK[index];
	}
	else if (type == "Q") {
		return (valuesROOK[index] + valuesBISHOP[index]) / 2.0;
	}
	else if (type == "K") {
		return valuesKING[index];
	}

	//b³¹d danych
	return 0;
}