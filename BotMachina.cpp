#include "BotMachina.h"
#include "Constants.h"
#include "Piece.h"
#include "Board.h"
#include <iostream>


BotMachina::BotMachina() {
	std::cout << "Def const\n";
}

BotMachina::BotMachina(ColorType color)
{
	this->color = color;
	std::cout << "ColorType color const\n";
}

BotMachina::~BotMachina()
{
	std::cout << "~BotMachina\n";
}

//todo: getMove(unsigned int depth,...)
Move BotMachina::getMove(std::string positionFEN) {
	//expected(positionFEN) [position] [botColor] ...
	Board board(positionFEN);
	std::vector<Move> allPossibleMoves = board.getAllPossibleMoves();

	//if no moves possible
	if (allPossibleMoves.size() == 0) {
		Move emptyMove;
		emptyMove.setEmpty();
		return emptyMove;
	}

	Move bestMove;
	double bestEval = 0;
	bool first = true;
	for (Move move : allPossibleMoves) {
		board.makeMove(move);
		int colorMultiplier = color == ColorType::LIGHT ? 1 : -1;
		double evalValue = this->evalPosition( board.getFEN() ) * colorMultiplier;

		//set first max
		if (first) {
			bestMove = move;
			bestEval = evalValue;
			first = false;
		}

		if (bestEval < evalValue) {
			bestEval = evalValue;
			bestMove = move;
		}
		
		board.undoMove(); // TODO // na razie reset przez inicjalizacje
	}

	return bestMove;
}

double BotMachina::evalPosition(std::string positionFEN) {
	// >0	white +
	// ==0	equal 0
	// <0	black -
	Board board(positionFEN);
	std::map<std::string, double> pieceValue;
	pieceValue["P"] = 1;
	pieceValue["B"] = 3;
	pieceValue["N"] = 3;
	pieceValue["R"] = 5;
	pieceValue["Q"] = 9;

	double evalPoints = 0;

	//for each piece on board count points
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (board.at(row, col) == nullptr) continue;
			
			//each piece
			std::string pieceCapitalType = board.at(row, col)->getCapitalType();
			int valueMultiplier = board.at(row, col)->getColor() == ColorType::LIGHT ? 1 : -1;

			if (pieceValue.count(pieceCapitalType)) {
				double value = pieceValue.at(pieceCapitalType);
				value *= VALUE_MAP::getValueAtPositioin(row, col, pieceCapitalType, color);
				evalPoints += value * valueMultiplier;
			}
		}
	}

	return evalPoints;
}

Move BotMachina::depthSearch(std::string positionFEN, int depth) {
	if (depth == 1) {
		//trywialny
		return getMove(positionFEN);
	}
	else {
		ColorType enemyColor = this->color == ColorType::LIGHT ? ColorType::DARK : ColorType::LIGHT;
		Board board(positionFEN); 
		std::vector<Move> possibleMoves = board.getAllPossibleMoves();
		std::vector<double> finalMovesEvaluations;

		//enemy response
		for (auto move : possibleMoves) {
			board.makeMove(move);
			std::vector<Move> possibleMovesEnemy = board.getAllPossibleMoves();

			for (auto enemyMove : possibleMovesEnemy) {
				board.makeMove(enemyMove);
				//odpowiedŸ na ruch przeciwnika
				//my* best reaction/response on enemy /\ move
				Move myResponse = this->depthSearch(board.getFEN(), depth - 1);
				board.makeMove(myResponse);
				//push_back FINAL EVALUATION
				finalMovesEvaluations.push_back(evalPosition(board.getFEN()));
				//std::cout << "Size: " << finalMovesEvaluations.size() << std::endl;
				//undo moves (my response and enemy move)
				board.undoMove();
				board.undoMove();
			}
			board.undoMove(); //clear (undo move / back to original state) 
		}

		int bestIndex = 0;
		for (int i = 0; i < possibleMoves.size(); i++) {
			if (finalMovesEvaluations[bestIndex] < finalMovesEvaluations[i])
				bestIndex = i;
		}

		return possibleMoves[bestIndex];
	}
}

//STEP 1
//Move BotMachina::getMove(std::string positionFEN) {
//	//expected(positionFEN) [position] [botColor] ...
//	Board board(positionFEN);
//	std::vector<Move> allPossibleMoves;
//	//---
//
//	for (int row = 0; row < 8; row++) {
//		for (int col = 0; col < 8; col++) {
//			//if bot's piece
//			if (board.at(row, col) != nullptr && board.at(row, col)->getColor() == color) {
//				std::vector<Move> piecePossibleMoves = board.getPossibleMoves(row, col, this->color);
//				//combine (insert into all moves) all moves and piece moves
//				allPossibleMoves.insert(allPossibleMoves.begin(), piecePossibleMoves.begin(), piecePossibleMoves.end());
//			}
//		}
//	}
//
//	//---
//	if (allPossibleMoves.size() == 0) {
//		Move emptyMove;
//		emptyMove.setEmpty();
//		return emptyMove;
//	}
//	return allPossibleMoves.at(rand() % allPossibleMoves.size());
//}