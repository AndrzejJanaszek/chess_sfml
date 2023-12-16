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
	bool isMoveOnBoard(int row, int col);

	sf::Vector2i enPassant;

	ColorType* activePlayerPtr;

	bool isLightKingCastlePossible;
	bool isLightQueenCastlePossible;
	bool isDarkKingCastlePossible;
	bool isDarkQueenCastlePossible;
public:
	Board(ColorType* refActivePlayer);
	virtual ~Board();

	//functions
	void initBoard();
	Piece* at(int row, int col);
	void print();

	void setActivePlayer(ColorType* refActivePlayer);
	
	bool isFriendlyPiece(int row, int col, ColorType activeColor);
	bool isEnemyPiece(int row, int col, ColorType activeColor);
	bool isFreeSquare(int row, int col);

	bool isEnPassant();
	sf::Vector2i getEnPassant();
	void setEnPassant(sf::Vector2i newEnPassant);
	void clearEnPassant();

	//row, col -> piece's cordinates
	std::vector<Move> getPossibleMoves(int row, int col, ColorType activeColor);

	void makeMove(sf::Vector2i from, sf::Vector2i dest);
	void makeMove(sf::Vector2i from, Move move);

	std::vector<sf::Vector2i> getSquaresUnderAttack(ColorType attackerColor, bool kingsFuture);
	std::vector<sf::Vector2i> getPieceView(int row, int col, ColorType activeColor, bool kingsFuture);

	std::vector<sf::Vector2i> getSquaresUnderAttack(ColorType attackerColor);
	std::vector<sf::Vector2i> getPieceView(int row, int col, ColorType activeColor);
};

