#pragma once
#include <string>
#include "SFML/Graphics.hpp"

struct PieceName
{
	static const char BLACK_KING;
	static const char BLACK_QUEEN;
	static const char BLACK_ROOK;
	static const char BLACK_KNIGHT;
	static const char BLACK_BISHOP;
	static const char BLACK_PAWN;

	static const char WHITE_KING;
	static const char WHITE_QUEEN;
	static const char WHITE_ROOK;
	static const char WHITE_KNIGHT;
	static const char WHITE_BISHOP;
	static const char WHITE_PAWN;

	static const char EMPTY;
};

struct TextureName : PieceName
{
	static const std::string DARK_SQUARE;
	static const std::string LIGHT_SQUARE;
	static const std::string SQUARE_SHADOW;
};

struct MovesPatterns
{
	/*static std::vector<sf::Vector2i> WHITE_PAWN;
	static std::vector<sf::Vector2i> BLACK_PAWN;*/
	static std::vector<sf::Vector2i> BISHOP;
	static std::vector<sf::Vector2i> KNIGHT;
	static std::vector<sf::Vector2i> ROOK;
	static std::vector<sf::Vector2i> QUEEN;
	static std::vector<sf::Vector2i> KING;
};

struct PieceValue {
	static std::map<char, int> value;
};

enum ColorType
{
	LIGHT,
	DARK
};

enum MoveType {
	EN_PASSANT,
	PAWN_LONG,
	KING_CASTLE,
	QUEEN_CASTLE,
	PROMOTION,
	NORMAL,
	TAKE,
	EMPTY
};

enum GameStatus {
	MAT,
	PAT,
	ON
};

class Move
{
public:
	Move();
	Move(sf::Vector2i from, sf::Vector2i destination, MoveType moveType);
	virtual ~Move();

	sf::Vector2i from;
	sf::Vector2i destination;
	MoveType moveType;

	void setEmpty();
	bool isEmpty();
};

class VALUE_MAP {
public:
	VALUE_MAP();
	virtual ~VALUE_MAP();

	//type (piece type) => CAPS : "P" "B" "N" "R" "K" "Q"
	static double getValueAtPositioin(int row, int col, char type, ColorType color);
};

/*
* 
* input : (row, col, type)
* int index = 8*row + col
* 
* if(){}
* 
* 
* 
WHITE
double* values = 
[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0.65, 0.75, 0.8, 0.8, 0.75, 0.65, 0, 0, 0, 0.85, 1, 1, 0.85, 0, 0, 0, 0.8, 0.9, 1, 1, 0.9, 0.85, 0, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

BISHOP
[0.3, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.3, 0.35, 0, 0.45, 0.35, 0.35, 0.45, 0, 0.35, 0, 0.6, 0.55, 0.4, 0.4, 0.55, 0.6, 0, 0.6, 0.75, 0.6, 0.6, 0.6, 0.6, 0.75, 0.6, 0.6, 0.75, 1, 0.65, 0.65, 1, 0.75, 0.6, 0.85, 0.9, 0.75, 1, 1, 0.75, 0.9, 0.85, 0.85, 1, 0.85, 1, 1, 0.85, 1, 0.85, 0.6, 0.75, 0.5, 0, 0, 0.5, 0.75, 0.6]

KNIGHT
[0.2, 0.3, 0.4, 0.4, 0.4, 0.4, 0.3, 0.2, 0.3, 0.5, 0.6, 0.6, 0.6, 0.6, 0.5, 0.3, 0.4, 0.6, 0.65, 0.65, 0.65, 0.65, 0.6, 0.4, 0.4, 0.6, 0.8, 1, 1, 0.8, 0.6, 0.4, 0.45, 0.8, 1, 1, 1, 1, 0.8, 0.45, 0.5, 0.85, 1, 1, 1, 1, 0.85, 0.5, 0.3, 0.6, 0.7, 0.7, 0.7, 0.7, 0.6, 0.3, 0.1, 0.3, 0, 0, 0, 0, 0.3, 0.1]

ROOK
[0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.4, 0.4, 0.35, 0.35, 0.4, 0.4, 0.6, 0.6, 0.45, 0.35, 0.3, 0.3, 0.35, 0.45, 0.6, 0.6, 0.5, 0.4, 0.3, 0.3, 0.4, 0, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.6, 0.9, 1, 1, 1, 1, 0.9, 0.6]


QUEEN
po��czenie go�ca i wie�y
�rednia arytmetyczna
(jedno + drugie) / 2

KING
[0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.4, 0.5, 0.5, 0.45, 0.2, 0.1, 0.2, 0.4, 0.7, 0.7, 0.7, 0.7, 0.5, 0.2, 0.2, 0, 0.7, 0.7, 0.7, 0.7, 0, 0.2, 0.45, 0.45, 0, 0.6, 0.6, 0, 0.45, 0, 0.7, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.7, 0.7, 0.7, 0.65, 0.4, 0.4, 0.45, 0.7, 0.7, 1, 1, 1, 0.7, 0, 0.7, 1, 1]
*/