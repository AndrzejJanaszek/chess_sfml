#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Constants.h"

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
	
	char* board;
	std::vector<std::string> fenHistory;
	void initVariables();
	bool isMoveOnBoard(int row, int col);

	sf::Vector2i enPassant;

	//ColorType* activePlayerPtr;
	ColorType activePlayer;

	bool isLightKingCastlePossible;
	bool isLightQueenCastlePossible;
	bool isDarkKingCastlePossible;
	bool isDarkQueenCastlePossible;
public:
	//Board(ColorType* refActivePlayer);
	Board(std::string fen);
	virtual ~Board();

	//functions
	void print();

	void initBoard();
	char at(int row, int col);
	void setAt(int row, int col, char type);
	ColorType getPieceColorAt(int row, int col);
	ColorType getPieceColor(char type);
	char getPieceCapsTypeAt(int row, int col);
	char getPieceCapsType(char type);

	void setActivePlayer(ColorType activePlayer);
	ColorType getActivePlayer();
	void swapActivePlayer();
	
	bool isFriendlyPiece(int row, int col, ColorType activeColor);
	bool isEnemyPiece(int row, int col, ColorType activeColor);
	bool isFreeSquare(int row, int col);

	bool isEnPassant();
	sf::Vector2i getEnPassant();
	void setEnPassant(sf::Vector2i newEnPassant);
	void clearEnPassant();

	//row, col -> piece's cordinates
	std::vector<Move> getPossibleMoves(int row, int col, ColorType activeColor);
	std::vector<Move> getAllPossibleMoves();


	void makeMove(sf::Vector2i from, sf::Vector2i dest);
	void makeMove(Move move);
	void undoMove();

	std::vector<sf::Vector2i> getSquaresUnderAttack(ColorType attackerColor, bool ignoreKing);
	std::vector<sf::Vector2i> getPieceView(int row, int col, ColorType activeColor, bool ignoreKing);

	std::vector<sf::Vector2i> getSquaresUnderAttack(ColorType attackerColor);
	std::vector<sf::Vector2i> getPieceView(int row, int col, ColorType activeColor);

	std::string getFEN();

	bool isCheck();
	//checkKingColor - color of king that will be checked/attacked
	/*bool isCheck(ColorType checkKingColor);*/

	GameStatus gameStatus();

	std::string getNotation(int row, int col);
	sf::Vector2i getCords(std::string notation);

};

