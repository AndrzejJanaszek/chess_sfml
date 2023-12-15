#include "Piece.h"
Piece::Piece(std::string fenType)
{
	//std::string tmp(1, fenType);
	this->type = fenType;

	if (PieceName::EMPTY.compare(fenType)) {
		sf::Sprite pieceSprite;
		pieceSprite.setScale(100 / 256.0f, 100 / 256.0f);
		this->sprite = pieceSprite;
	}
}

Piece::~Piece()
{
}

ColorType Piece::getColor() {
	//CAP	65 90	WHITE
	//small 97 122	BLACK
	if (65 <= type[0] && type[0] <= 90)
		return ColorType::LIGHT;
	return ColorType::DARK;
}