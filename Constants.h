#pragma once
#include <string>
#include "SFML/Graphics.hpp"

struct PieceName
{
	static const std::string BLACK_KING;
	static const std::string BLACK_QUEEN;
	static const std::string BLACK_ROOK;
	static const std::string BLACK_KNIGHT;
	static const std::string BLACK_BISHOP;
	static const std::string BLACK_PAWN;

	static const std::string WHITE_KING;
	static const std::string WHITE_QUEEN;
	static const std::string WHITE_ROOK;
	static const std::string WHITE_KNIGHT;
	static const std::string WHITE_BISHOP;
	static const std::string WHITE_PAWN;

	static const std::string EMPTY;
};


struct TextureName : PieceName
{
	static const std::string DARK_SQUARE;
	static const std::string LIGHT_SQUARE;
	static const std::string SQUARE_SHADOW;
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
	TAKE
};

class Move
{
public:
	Move();
	Move(sf::Vector2i pos, MoveType moveType);
	virtual ~Move();

	sf::Vector2i position;
	MoveType moveType;
};