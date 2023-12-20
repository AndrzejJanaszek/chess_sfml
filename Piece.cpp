#include "Piece.h"
Piece::Piece(std::string fenType)
{
	//std::string tmp(1, fenType);
	this->type = fenType;
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

std::string Piece::getCapitalType(){
	//CAP	65 90	WHITE
	//small 97 122	BLACK
	if (65 <= type[0] && type[0] <= 90) {
		//white
		return this->type;
	}
	return std::string(1, this->type[0] - 32);
}