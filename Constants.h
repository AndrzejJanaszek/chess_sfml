#pragma once
#include <string>

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

//const std::string PieceName::BLACK_KING = "k";
//const std::string PieceName::BLACK_QUEEN = "q";
//const std::string PieceName::BLACK_ROOK = "r";
//const std::string PieceName::BLACK_KNIGHT = "n";
//const std::string PieceName::BLACK_BISHOP = "b";
//const std::string PieceName::BLACK_PAWN = "p";
//const std::string PieceName::WHITE_KING = "K";
//const std::string PieceName::WHITE_QUEEN = "Q";
//const std::string PieceName::WHITE_ROOK = "R";
//const std::string PieceName::WHITE_KNIGHT = "N";
//const std::string PieceName::WHITE_BISHOP = "B";
//const std::string PieceName::WHITE_PAWN = "P";
//const std::string PieceName::EMPTY = "-";


struct TextureName : PieceName
{
	static const std::string DARK_SQUARE;
	static const std::string LIGHT_SQUARE;
};
//const std::string TextureName::DARK_SQUARE = "dark_square";
//const std::string TextureName::LIGHT_SQUARE = "light_square";

enum ColorType
{
	LIGHT,
	DARK
};