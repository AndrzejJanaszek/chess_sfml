#pragma once
#include "Piece.h"

class Board
{
private:
	//FEN
	//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
	//PGN ta druga
	// 
	// x,y - vertical:y horizontal:
	//(0,0) (0,1) ... (0,7)
	//(1,0) (1,1) ... (1,7)
	// ...   ...  ...  ...
	//(7,0) (7,1) ... (7,7)

	// 0,0 1,0 2,0 ...
	// [0] [1] [2] [3] [4] [5] [6] ... [n] 
	Piece** board;
	void initVariables();
public:
	Board();
	virtual ~Board();

	//functions
	void initBoard();
	Piece* at(int row, int col);
	void print();
	bool isFriendlyPiece(int row, int col, ColorType color);

	//row, col -> piece's cordinates
	std::vector<sf::Vector2i> getPossibleMoves(int row, int col, ColorType activeColor);
};
