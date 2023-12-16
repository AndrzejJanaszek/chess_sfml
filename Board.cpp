#include "Board.h"
#include <string>
#include <iostream>

Board::Board(ColorType* refActivePlayer)
{
	initVariables();
	setActivePlayer(refActivePlayer);

	this->initBoard();
	this->print();
}

Board::~Board()
{
	delete[] board;
}

void Board::initVariables() {
	//this->board = nullptr;
	this->board = new Piece*[8*8];
	this->enPassant = sf::Vector2i(-1, -1);
}

void Board::initBoard() {
	//if empty: -
	//const char empty = '-';
	//std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	std::string fen = "1r2kbnr/pp1ppppp/2p2q2/1NP5/B2b4/1n3NQ1/PP1P1PPP/R3KB1R w KQk - 0 1";
	//std::string fen = "3q4/8/6P1/PK1N1k2/PP1p4/p5np/4pP1p/5r2 w - - 0 1";<

	std::vector<std::string> fenSplited;

	//split fen by ' ' into 6 strings
	std::string chunkStr = "";
	for (int i = 0; i < fen.length(); i++) {
		if (fen[i] == ' ' || i == fen.length()-1) {
			fenSplited.push_back(chunkStr);
			chunkStr = "";
		}
		else {
			chunkStr += fen[i];
		}
	}

	//FEN POSITION STRING NR 1
	int j = 0;
	std::string fenPosition = fenSplited[0];
	for (int i = 0; i < fenPosition.length(); i++) {
		if (j >= 64)
			break;
		//skip /
		if (fenPosition[i] == '/') {
			continue;
		}
		//if digit (<57) will never be 9
		//fill empty
		if (48 <= fenPosition[i] && fenPosition[i] <= 57) {
			for (int ii = 0; ii < fenPosition[i] - 48; ii++) {
				board[j] = nullptr;
				j++;
			}
			continue;
		}

		//piece
		board[j] = new Piece( std::string(1, fenPosition[i]) );
		int row = j / 8;
		int col = j % 8;
		board[j]->sprite.setPosition(100 * col, 100 * row);
		j++;
	}

	//FEN TURN STRING NR 2
	if (fenSplited[1] == "w") {
		*activePlayerPtr = ColorType::LIGHT;
	}
	else {
		*activePlayerPtr = ColorType::DARK;
	}
}

Piece* Board::at(int row, int col) {
	return board[row * 8 + col];
}

void Board::print() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->board[i * 8 + j] != nullptr) {
				std::cout << this->board[i*8+j]->type;
			}
			else {
				std::cout << ".";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\nKoniec Wypisu\n";
}

void Board::setActivePlayer(ColorType* refActivePlayer) {
	this->activePlayerPtr = refActivePlayer;
}

bool Board::isFriendlyPiece(int row, int col, ColorType activeColor) {
	if (this->isFreeSquare(row,col)) return false;

	if (this->at(row, col)->getColor() == activeColor)
		return true;
	return false;
}

bool Board::isEnemyPiece(int row, int col, ColorType activeColor) {
	if (this->isFreeSquare(row, col)) return false;

	if (this->at(row, col)->getColor() != activeColor)
		return true;
	return false;
}

bool Board::isFreeSquare(int row, int col) {
	if (this->at(row, col) == nullptr) return true;

	return false;
}

std::vector<Move> Board::getPossibleMoves(int row, int col, ColorType activeColor) {
	std::vector<Move> possibleMoves;
	if (this->at(row, col) == nullptr) return possibleMoves;

	std::string pieceType = this->at(row, col)->type;
	if (pieceType[0] >= 97) pieceType = pieceType[0] - 32;
	//pawn
	if (pieceType == "P") {
		int firstPawnRow = activeColor == ColorType::LIGHT ? 6 : 1;
		//for pawn
		int colorDirectionMultiplier = activeColor == ColorType::LIGHT ? -1 : 1;
		sf::Vector2i attack1 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col - 1);
		sf::Vector2i attack2 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col + 1);
		sf::Vector2i move1 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col);
		sf::Vector2i move2= sf::Vector2i(row + 2 * colorDirectionMultiplier, col);

		//attacks
		//if onBoard AND isEnemy
		if ( this->isMoveOnBoard(attack1.x, attack1.y) ) {
			if (enPassant.x == attack1.x && enPassant.y == attack1.y) {
				possibleMoves.push_back(Move(attack1, MoveType::EN_PASSANT) );
			}else if ( isEnemyPiece(attack1.x, attack1.y, activeColor)) {
				possibleMoves.push_back(Move(attack1, MoveType::TAKE) );
			}
		}
		if (this->isMoveOnBoard(attack2.x, attack2.y)) {
			if (enPassant.x == attack2.x && enPassant.y == attack2.y) {
				possibleMoves.push_back(Move(attack2, MoveType::EN_PASSANT));
			}else if (isEnemyPiece(attack2.x, attack2.y, activeColor)) {
				possibleMoves.push_back(Move(attack2, MoveType::TAKE));
			}
		}

		//move1
		//if onBoard AND isFreeSquare
		if (this->isMoveOnBoard(move1.x, move1.y) && isFreeSquare(move1.x, move1.y)) {
			possibleMoves.push_back(Move(move1, MoveType::NORMAL) );

			//move2 PAWN_LONG
			if (firstPawnRow == row) {
				if (this->isMoveOnBoard(move2.x, move2.y) && isFreeSquare(move2.x, move2.y)) {
					possibleMoves.push_back(Move(move2, MoveType::PAWN_LONG));
				}
			}
		}
	}

	if (pieceType == "N") {
		std::vector<sf::Vector2i> movesTemp;
		movesTemp.push_back(sf::Vector2i(row + 2, col + 1));
		movesTemp.push_back(sf::Vector2i(row + 2, col - 1));
		movesTemp.push_back(sf::Vector2i(row + 1, col + 2));
		movesTemp.push_back(sf::Vector2i(row + 1, col - 2));
		movesTemp.push_back(sf::Vector2i(row - 1, col + 2));
		movesTemp.push_back(sf::Vector2i(row - 1, col - 2));
		movesTemp.push_back(sf::Vector2i(row - 2, col + 1));
		movesTemp.push_back(sf::Vector2i(row - 2, col - 1));

		for (auto moveT : movesTemp) {
			//if not on board continue
			if (!isMoveOnBoard(moveT.x, moveT.y)) continue;

			if (isFreeSquare(moveT.x, moveT.y)) {
				possibleMoves.push_back(Move(moveT, MoveType::NORMAL));
			}else if (isEnemyPiece(moveT.x, moveT.y, activeColor) ) {
				possibleMoves.push_back(Move(moveT, MoveType::TAKE));
			}
		}
	}

	if (pieceType == "R") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 0));
		moveDirections.push_back(sf::Vector2i(-1, 0));
		moveDirections.push_back(sf::Vector2i(0, 1));
		moveDirections.push_back(sf::Vector2i(0, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					possibleMoves.push_back(Move(move, MoveType::NORMAL)); 
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(Move(move,MoveType::TAKE));

					break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "B") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 1));
		moveDirections.push_back(sf::Vector2i(1, -1));
		moveDirections.push_back(sf::Vector2i(-1, 1));
		moveDirections.push_back(sf::Vector2i(-1, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					possibleMoves.push_back(Move(move, MoveType::NORMAL));
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(Move(move, MoveType::TAKE));

					break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "Q") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 0));
		moveDirections.push_back(sf::Vector2i(-1, 0));
		moveDirections.push_back(sf::Vector2i(0, 1));
		moveDirections.push_back(sf::Vector2i(0, -1));
		moveDirections.push_back(sf::Vector2i(1, 1));
		moveDirections.push_back(sf::Vector2i(1, -1));
		moveDirections.push_back(sf::Vector2i(-1, 1));
		moveDirections.push_back(sf::Vector2i(-1, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					possibleMoves.push_back(Move(move, MoveType::NORMAL));
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(Move(move, MoveType::TAKE));

					break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "K") {
		ColorType enemyColor = activeColor == ColorType::LIGHT ? ColorType::DARK : ColorType::LIGHT;
		std::vector<sf::Vector2i> movesTemp;
		std::vector<sf::Vector2i> squaresUnderAttack = getSquaresUnderAttack(enemyColor, true);
		movesTemp.push_back(sf::Vector2i(row + 1, col));
		movesTemp.push_back(sf::Vector2i(row - 1, col));
		movesTemp.push_back(sf::Vector2i(row, col + 1));
		movesTemp.push_back(sf::Vector2i(row, col - 1));
		movesTemp.push_back(sf::Vector2i(row + 1, col - 1));
		movesTemp.push_back(sf::Vector2i(row + 1, col + 1));
		movesTemp.push_back(sf::Vector2i(row - 1, col - 1));
		movesTemp.push_back(sf::Vector2i(row + 1, col - 1));

		for (auto move : movesTemp) {
			if (!isMoveOnBoard(move.x, move.y)) continue;
			bool containsMove = false;
			//is square under attack
			for (auto squareUnderAttack : squaresUnderAttack) {
				if (move.x == squareUnderAttack.x && move.y == squareUnderAttack.y) {
					containsMove = true; 
					break; 
				}
			}

			//if not underAttack and is not friendly piece
			if (!containsMove && !isFriendlyPiece(move.x, move.y, activeColor)) {
				//after move will king be under attack
				possibleMoves.push_back(Move(move, MoveType::NORMAL));
			}
		}
		
	}

	return possibleMoves;
}

bool Board::isMoveOnBoard(int row, int col) {
	if (0 <= row && row < 8 &&
		0 <= col && col < 8) return true;

	return false;
}

bool Board::isEnPassant() {
	if (this->enPassant.x >= 0 && this->enPassant.y >= 0 && this->enPassant.x < 8 && this->enPassant.y < 8)
		return true;
	return false;
}

sf::Vector2i Board::getEnPassant() {
	return this->enPassant;
}

void Board::setEnPassant(sf::Vector2i newEnPassant) {
	this->enPassant = newEnPassant;
}

void Board::clearEnPassant() {
	this->enPassant = sf::Vector2i(-1,-1);
}

void Board::makeMove(sf::Vector2i from, sf::Vector2i dest) {
	board[dest.x * 8 + dest.y] = this->at(from.x, from.y);
	board[from.x * 8 + from.y] = nullptr;
}

void Board::makeMove(sf::Vector2i from, Move move) {
	if (MoveType::PAWN_LONG == move.moveType) {
		int targetRow = (from.x - move.position.x) / 2;
		this->enPassant = sf::Vector2i(from.x-targetRow,from.y);
		std::cout << "LONG_PAWN\n";
		//1 -> 3 3-1=2 / 2 = 1   start
		//6 -> 4 4-6=-2 / 2 = -1
	}
	else if (MoveType::EN_PASSANT == move.moveType) {
		//TODO
		board[from.x * 8 + move.position.y] = nullptr;		
	}
	else if(MoveType::LONG_CASTLE == move.moveType) {
		//TODO
	}
	else if (MoveType::SHORT_CASTLE == move.moveType) {
		//TODO
	}
	else if (MoveType::PROMOTION == move.moveType) {
		//TODO
	}

	makeMove(from, move.position);
}

std::vector<sf::Vector2i> Board::getSquaresUnderAttack(ColorType attackerColor, bool kingsFuture) {
	std::vector<sf::Vector2i> squares;
	bool* table = new bool[8 * 8]{0};
	// tableAt ( row*8 + col )
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (this->at(row, col) != nullptr && this->at(row,col)->getColor() == attackerColor) {
				//for each square that piece see / is under piece's attack
				for (auto viewedSquare : getPieceView(row, col, attackerColor, kingsFuture)) {
					table[viewedSquare.x * 8 + viewedSquare.y] = true;
				}
			}
		}
	}
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (table[row * 8 + col]) squares.push_back(sf::Vector2i(row, col));

			std::cout << table[row * 8 + col];
		}
		std::cout <<"\n";
	}

	return squares;
}

std::vector<sf::Vector2i> Board::getPieceView(int row, int col, ColorType activeColor, bool kingsFuture){
	std::vector<sf::Vector2i> viewedSquares;
	if (this->at(row, col) == nullptr) return viewedSquares;

	std::string pieceType = this->at(row, col)->type;
	if (pieceType[0] >= 97) pieceType = pieceType[0] - 32;
	//pawn
	if (pieceType == "P") {
		//for pawn
		int colorDirectionMultiplier = activeColor == ColorType::LIGHT ? -1 : 1;
		sf::Vector2i attack1 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col - 1);
		sf::Vector2i attack2 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col + 1);

		//attacks
		//if onBoard AND isEnemy
		if (this->isMoveOnBoard(attack1.x, attack1.y)) {
			if (isEnemyPiece(attack1.x, attack1.y, activeColor) || (enPassant.x == attack1.x && enPassant.y == attack1.y)) {
				viewedSquares.push_back(attack1);
			}
		}
		if (this->isMoveOnBoard(attack2.x, attack2.y)) {
			if (isEnemyPiece(attack2.x, attack2.y, activeColor) || (enPassant.x == attack2.x && enPassant.y == attack2.y)) {
				viewedSquares.push_back(attack2);
			}
		}
	}

	if (pieceType == "N") {
		std::vector<sf::Vector2i> movesTemp;
		movesTemp.push_back(sf::Vector2i(row + 2, col + 1));
		movesTemp.push_back(sf::Vector2i(row + 2, col - 1));
		movesTemp.push_back(sf::Vector2i(row + 1, col + 2));
		movesTemp.push_back(sf::Vector2i(row + 1, col - 2));
		movesTemp.push_back(sf::Vector2i(row - 1, col + 2));
		movesTemp.push_back(sf::Vector2i(row - 1, col - 2));
		movesTemp.push_back(sf::Vector2i(row - 2, col + 1));
		movesTemp.push_back(sf::Vector2i(row - 2, col - 1));

		for (auto moveT : movesTemp) {
			//if not on board continue
			if (!isMoveOnBoard(moveT.x, moveT.y)) continue;
			viewedSquares.push_back(moveT);
		}
	}

	if (pieceType == "R") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 0));
		moveDirections.push_back(sf::Vector2i(-1, 0));
		moveDirections.push_back(sf::Vector2i(0, 1));
		moveDirections.push_back(sf::Vector2i(0, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					viewedSquares.push_back(move);
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor) || this->isFriendlyPiece(move.x, move.y, activeColor)) {
					viewedSquares.push_back(move);
					
					//if kingFuture = true skip KING
					bool isTargetKing = (this->at(move.x, move.y)->type == "K" || this->at(move.x, move.y)->type == "k");
					//if kingsFuture==true and target is king then DO NOT BREAK
					if (!(kingsFuture && isTargetKing))
						break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "B") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 1));
		moveDirections.push_back(sf::Vector2i(1, -1));
		moveDirections.push_back(sf::Vector2i(-1, 1));
		moveDirections.push_back(sf::Vector2i(-1, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					viewedSquares.push_back(move);
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor) || this->isFriendlyPiece(move.x, move.y, activeColor)) {
					viewedSquares.push_back(move);

					//if kingFuture = true skip KING
					bool isTargetKing = (this->at(move.x, move.y)->type == "K" || this->at(move.x, move.y)->type == "k");
					//if kingsFuture==true and target is king then DO NOT BREAK
					if (!(kingsFuture && isTargetKing))
						break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "Q") {
		std::vector<sf::Vector2i> moveDirections;
		moveDirections.push_back(sf::Vector2i(1, 0));
		moveDirections.push_back(sf::Vector2i(-1, 0));
		moveDirections.push_back(sf::Vector2i(0, 1));
		moveDirections.push_back(sf::Vector2i(0, -1));
		moveDirections.push_back(sf::Vector2i(1, 1));
		moveDirections.push_back(sf::Vector2i(1, -1));
		moveDirections.push_back(sf::Vector2i(-1, 1));
		moveDirections.push_back(sf::Vector2i(-1, -1));

		//for each direction
		for (auto mDir : moveDirections) {
			int multiplier = 1;
			sf::Vector2i move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					viewedSquares.push_back(move);
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor) || this->isFriendlyPiece(move.x, move.y, activeColor)) {
					viewedSquares.push_back(move);

					//if kingFuture = true skip KING
					bool isTargetKing = (this->at(move.x, move.y)->type == "K" || this->at(move.x, move.y)->type == "k");
					//if kingsFuture==true and target is king then DO NOT BREAK
					if (!(kingsFuture && isTargetKing))
						break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "K") {
		viewedSquares.push_back(sf::Vector2i(row + 1, col));
		viewedSquares.push_back(sf::Vector2i(row - 1, col));
		viewedSquares.push_back(sf::Vector2i(row, col + 1));
		viewedSquares.push_back(sf::Vector2i(row, col - 1));
		viewedSquares.push_back(sf::Vector2i(row + 1, col - 1));
		viewedSquares.push_back(sf::Vector2i(row + 1, col + 1));
		viewedSquares.push_back(sf::Vector2i(row - 1, col - 1));
		viewedSquares.push_back(sf::Vector2i(row + 1, col - 1));
	}

	return viewedSquares;
}

std::vector<sf::Vector2i> Board::getSquaresUnderAttack(ColorType attackerColor) {
	return getSquaresUnderAttack(attackerColor, false);
}

std::vector<sf::Vector2i> Board::getPieceView(int row, int col, ColorType activeColor) {
	return getPieceView(row, col, activeColor, false);
}