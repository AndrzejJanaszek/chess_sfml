#include "BotMachina.h"
#include "Constants.h"
#include "Board.h"
#include <iostream>


BotMachina::BotMachina() {
}

BotMachina::BotMachina(ColorType color)
{
	this->color = color;
}

BotMachina::~BotMachina()
{
}

Move BotMachina::getMove(Board &board) {
	//expected(positionFEN) [position] [botColor] ...
	std::vector<Move> allPossibleMoves = board.getAllPossibleMoves();

	//if no moves possible
	if (allPossibleMoves.size() == 0) {
		Move emptyMove;
		emptyMove.setEmpty();
		return emptyMove;
	}

	Move bestMove;
	int bestEval = 0;
	bool first = true;
	for (Move &move : allPossibleMoves) {
		board.makeMove(move);
		int colorMultiplier = color == ColorType::LIGHT ? 1 : -1;
		int evalValue = this->evalPosition(board) * colorMultiplier;

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
		
		board.undoMove();
	}

	return bestMove;
}

int BotMachina::evalPosition(Board &board) {
	// >0	white +
	// ==0	equal 0
	// <0	black -

	int evalPoints = 0;

	//for each piece on board count points
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (board.at(row, col) == PieceName::EMPTY) continue;
			
			//each piece
			char pieceCapitalType = board.getPieceCapsTypeAt(row, col);
			int valueMultiplier = board.getPieceColorAt(row, col) == ColorType::LIGHT ? 1 : -1;

			int value = PieceValue::value.at(pieceCapitalType);
			//value *= VALUE_MAP::getValueAtPositioin(row, col, pieceCapitalType, color);
			evalPoints += value * valueMultiplier;
		}
	}

	return evalPoints;
}

Move BotMachina::depthSearch(Board& board, int depth) {
	if (depth == 1) {
		//trywialny
		return getMove(board);
	}
	else {
		ColorType enemyColor = this->color == ColorType::LIGHT ? ColorType::DARK : ColorType::LIGHT;
		std::vector<Move> possibleMoves = board.getAllPossibleMoves();
		std::vector<double> finalMovesEvaluations;

		//enemy response
		for (auto &move : possibleMoves) {
			board.makeMove(move);
			std::vector<Move> possibleMovesEnemy = board.getAllPossibleMoves();

			for (auto &enemyMove : possibleMovesEnemy) {
				board.makeMove(enemyMove);
				//odpowied� na ruch przeciwnika
				//my* best reaction/response on enemy /\ move
				Move myResponse = this->depthSearch(board, depth - 1);
				board.makeMove(myResponse);
				//push_back FINAL EVALUATION
				finalMovesEvaluations.push_back( evalPosition(board) );
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