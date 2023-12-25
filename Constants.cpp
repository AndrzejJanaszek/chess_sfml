#include "Constants.h"
#include <string>
#include "SFML/Graphics.hpp"

const char PieceName::BLACK_KING	= 'k';
const char PieceName::BLACK_QUEEN	= 'q';
const char PieceName::BLACK_ROOK	= 'r';
const char PieceName::BLACK_KNIGHT	= 'n';
const char PieceName::BLACK_BISHOP	= 'b';
const char PieceName::BLACK_PAWN	= 'p';
const char PieceName::WHITE_KING	= 'K';
const char PieceName::WHITE_QUEEN	= 'Q';
const char PieceName::WHITE_ROOK	= 'R';
const char PieceName::WHITE_KNIGHT	= 'N';
const char PieceName::WHITE_BISHOP	= 'B';
const char PieceName::WHITE_PAWN	= 'P';
const char PieceName::EMPTY			= '-';

const std::string TextureName::DARK_SQUARE = "dark_square";
const std::string TextureName::LIGHT_SQUARE = "light_square";
const std::string TextureName::SQUARE_SHADOW = "square_shadow";

std::vector<sf::Vector2i> MovesPatterns::BISHOP = {
	sf::Vector2i(1, 1),
	sf::Vector2i(1, -1),
	sf::Vector2i(-1, 1),
	sf::Vector2i(-1, -1)
};

std::vector<sf::Vector2i> MovesPatterns::KNIGHT = {
	sf::Vector2i(2, -1),
	sf::Vector2i(2, 1),
	sf::Vector2i(1, 2),
	sf::Vector2i(1, -2),
	sf::Vector2i(-1, 2),
	sf::Vector2i(-1, -2),
	sf::Vector2i(-2, 1),
	sf::Vector2i(-2, -1)
};

std::vector<sf::Vector2i> MovesPatterns::ROOK = {
	sf::Vector2i(1, 0),
	sf::Vector2i(-1, 0),
	sf::Vector2i(0, 1),
	sf::Vector2i(0, -1)
};

std::vector<sf::Vector2i> MovesPatterns::QUEEN = {
	sf::Vector2i(1, 0),
	sf::Vector2i(-1, 0),
	sf::Vector2i(0, 1),
	sf::Vector2i(0, -1),
	sf::Vector2i(1, 1),
	sf::Vector2i(1, -1),
	sf::Vector2i(-1, 1),
	sf::Vector2i(-1, -1)
};



std::vector<sf::Vector2i> MovesPatterns::KING = { MovesPatterns::QUEEN };
//std::vector<sf::Vector2i> KING = {
//	sf::Vector2i(1, 0),
//	sf::Vector2i(-1, 0),
//	sf::Vector2i(0, 1),
//	sf::Vector2i(0, -1),
//	sf::Vector2i(1, 1),
//	sf::Vector2i(1, -1),
//	sf::Vector2i(-1, 1),
//	sf::Vector2i(-1, -1)
//};

std::map<char, int> PieceValue::value = {
	std::pair<char, int>(PieceName::WHITE_PAWN, 100),
	std::pair<char, int>(PieceName::WHITE_BISHOP, 300),
	std::pair<char, int>(PieceName::WHITE_KNIGHT, 300),
	std::pair<char, int>(PieceName::WHITE_ROOK, 500),
	std::pair<char, int>(PieceName::WHITE_QUEEN, 900)
};

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
double VALUE_MAP::getValueAtPositioin(int row, int col, char type, ColorType color) {
	if (color == ColorType::DARK) row = 7 - row;

	int index = 8 * row + col;
	double valuesPAWN[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0.65, 0.75, 0.8, 0.8, 0.75, 0.65, 0, 0, 0, 0.85, 1, 1, 0.85, 0, 0, 0, 0.8, 0.9, 1, 1, 0.9, 0.85, 0, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	double valuesBISHOP[] = { 0.3, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.3, 0.35, 0, 0.45, 0.35, 0.35, 0.45, 0, 0.35, 0, 0.6, 0.55, 0.4, 0.4, 0.55, 0.6, 0, 0.6, 0.75, 0.6, 0.6, 0.6, 0.6, 0.75, 0.6, 0.6, 0.75, 1, 0.65, 0.65, 1, 0.75, 0.6, 0.85, 0.9, 0.75, 1, 1, 0.75, 0.9, 0.85, 0.85, 1, 0.85, 1, 1, 0.85, 1, 0.85, 0.6, 0.75, 0.5, 0, 0, 0.5, 0.75, 0.6 };
	double valuesKNIGHT[] = { 0.2, 0.3, 0.4, 0.4, 0.4, 0.4, 0.3, 0.2, 0.3, 0.5, 0.6, 0.6, 0.6, 0.6, 0.5, 0.3, 0.4, 0.6, 0.65, 0.65, 0.65, 0.65, 0.6, 0.4, 0.4, 0.6, 0.8, 1, 1, 0.8, 0.6, 0.4, 0.45, 0.8, 1, 1, 1, 1, 0.8, 0.45, 0.5, 0.85, 1, 1, 1, 1, 0.85, 0.5, 0.3, 0.6, 0.7, 0.7, 0.7, 0.7, 0.6, 0.3, 0.1, 0.3, 0, 0, 0, 0, 0.3, 0.1 };
	double valuesROOK[] = { 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.4, 0.4, 0.35, 0.35, 0.4, 0.4, 0.6, 0.6, 0.45, 0.35, 0.3, 0.3, 0.35, 0.45, 0.6, 0.6, 0.5, 0.4, 0.3, 0.3, 0.4, 0, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.6, 0.9, 1, 1, 1, 1, 0.9, 0.6 };
	double valuesKING[] = { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.4, 0.5, 0.5, 0.45, 0.2, 0.1, 0.2, 0.4, 0.7, 0.7, 0.7, 0.7, 0.5, 0.2, 0.2, 0, 0.7, 0.7, 0.7, 0.7, 0, 0.2, 0.45, 0.45, 0, 0.6, 0.6, 0, 0.45, 0, 0.7, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.7, 0.7, 0.7, 0.65, 0.4, 0.4, 0.45, 0.7, 0.7, 1, 1, 1, 0.7, 0, 0.7, 1, 1 };

	if (type == PieceName::WHITE_PAWN) {
		return valuesPAWN[index];
	}
	else if (type == PieceName::WHITE_BISHOP) {
		return valuesBISHOP[index];
	}
	else if (type == PieceName::WHITE_KNIGHT) {
		return valuesKNIGHT[index];
	}
	else if (type == PieceName::WHITE_ROOK) {
		return valuesROOK[index];
	}
	else if (type == PieceName::WHITE_QUEEN) {
		return (valuesROOK[index] + valuesBISHOP[index]) / 2.0;
	}
	else if (type == PieceName::WHITE_KING) {
		return valuesKING[index];
	}

	//b³¹d danych
	return 0;
}