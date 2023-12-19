#include "Board.h"
#include <string>
#include <iostream>

//Board::Board(ColorType* refActivePlayer)
//{
//	initVariables();
//	setActivePlayer(refActivePlayer);
//
//	this->initBoard();
//	this->print();
//}

Board::Board(std::string fen)
{
	this->fen = fen;
	this->initVariables();
	//setActivePlayer(refActivePlayer);

	this->initBoard();
	//this->print();
}

Board::~Board()
{
	delete[] board;
}

void Board::initVariables() {
	//this->board = nullptr;
	this->board = new Piece*[8*8];
	this->enPassant = sf::Vector2i(-1, -1);

	this->isDarkKingCastlePossible = false;
	this->isDarkQueenCastlePossible = false;
	this->isLightKingCastlePossible = false;
	this->isLightQueenCastlePossible = false;
}

void Board::initBoard() {
	//if empty: -
	//const char empty = '-';
	//std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	//std::string fen = "1r2kbnr/pp1ppppp/2p2q2/1NP5/B2b4/1n3NQ1/PP1P1PPP/R3KB1R w KQk - 0 1";
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
		this->activePlayer = ColorType::LIGHT;
	}
	else {
		this->activePlayer = ColorType::DARK;
	}

	//FEN CASTLES STRING NR 3
	std::string castlesFen = fenSplited[2];
	for (int i = 0; i < castlesFen.length(); i++) {
		if (castlesFen[i] == 'K') {
			this->isLightKingCastlePossible = true;
		}
		else if (castlesFen[i] == 'Q') {
			this->isLightQueenCastlePossible = true;
		}
		else if (castlesFen[i] == 'k') {
			this->isDarkKingCastlePossible = true;
		}
		else if (castlesFen[i] == 'q') {
			this->isDarkQueenCastlePossible = true;
		}
	}

	//FEN EN PASSANT TODO STRING NR 4

	//FEN HALF COUNT TODO STRING NR 5

	//FEN COUTN TODO STRING NR 6
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

void Board::setActivePlayer(ColorType activePlayer) {
	this->activePlayer = activePlayer;
}

ColorType Board::getActivePlayer() {
	return this->activePlayer;
}

void Board::swapActivePlayer() {
	this->activePlayer = this->activePlayer == ColorType::LIGHT ? ColorType::DARK : ColorType::LIGHT;
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

//int row, int col => V2::from
std::vector<Move> Board::getPossibleMoves(int row, int col, ColorType activeColor) {
	std::vector<Move> possibleMoves;
	if (isFreeSquare(row,col)) return possibleMoves;

	sf::Vector2i from(row, col);
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
				possibleMoves.push_back(Move(from, attack1, MoveType::EN_PASSANT) );
			}else if ( isEnemyPiece(attack1.x, attack1.y, activeColor)) {
				possibleMoves.push_back(Move(from, attack1, MoveType::TAKE) );
			}
		}
		if (this->isMoveOnBoard(attack2.x, attack2.y)) {
			if (enPassant.x == attack2.x && enPassant.y == attack2.y) {
				possibleMoves.push_back(Move(from, attack2, MoveType::EN_PASSANT));
			}else if (isEnemyPiece(attack2.x, attack2.y, activeColor)) {
				possibleMoves.push_back(Move(from, attack2, MoveType::TAKE));
			}
		}

		//move1
		//if onBoard AND isFreeSquare
		if (this->isMoveOnBoard(move1.x, move1.y) && isFreeSquare(move1.x, move1.y)) {
			possibleMoves.push_back(Move(from, move1, MoveType::NORMAL) );

			//move2 PAWN_LONG
			if (firstPawnRow == row) {
				if (this->isMoveOnBoard(move2.x, move2.y) && isFreeSquare(move2.x, move2.y)) {
					possibleMoves.push_back(Move(from, move2, MoveType::PAWN_LONG));
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
				possibleMoves.push_back(Move(from, moveT, MoveType::NORMAL));
			}else if (isEnemyPiece(moveT.x, moveT.y, activeColor) ) {
				possibleMoves.push_back(Move(from, moveT, MoveType::TAKE));
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
					possibleMoves.push_back(Move(from, move, MoveType::NORMAL));
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(Move(from, move,MoveType::TAKE));

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
					possibleMoves.push_back(Move(from, move, MoveType::NORMAL));
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(Move(from, move, MoveType::TAKE));

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
					possibleMoves.push_back(Move(from, move, MoveType::NORMAL));
				}
				else if (this->isFriendlyPiece(move.x, move.y, activeColor)) {
					break;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor)) {
					possibleMoves.push_back(Move(from, move, MoveType::TAKE));

					break;
				}

				multiplier++;
				move = sf::Vector2i(row + mDir.x * multiplier, col + mDir.y * multiplier);
			}
		}
	}

	if (pieceType == "K") {
		ColorType enemyColor = activeColor == ColorType::LIGHT ? ColorType::DARK : ColorType::LIGHT;
		std::vector<sf::Vector2i> squaresUnderAttack = getSquaresUnderAttack(enemyColor, true);
		std::vector<sf::Vector2i> movesTemp;
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
				possibleMoves.push_back(Move(from, move, MoveType::NORMAL));
			}
		}
		

		//____CASTLES____
		//LIGHT values
		int row = 7;
		bool queenPossible = this->isLightQueenCastlePossible;
		bool kingPossible = this->isLightKingCastlePossible;
		//change values if DARK
		if (activeColor == ColorType::DARK) {
			row = 0;
			kingPossible = this->isDarkKingCastlePossible;
			queenPossible = this->isDarkQueenCastlePossible;
		}
//_____________________________

		bool columnsAttackStatus[8] = { 0 };
		//QUEEN col [0,1,2,3]
		//KING col	[5,6,7]
		// + is king under attack [4]

		//free squres check
		bool queenFree = isFreeSquare(row, 1) && isFreeSquare(row, 2) && isFreeSquare(row, 3);
		bool kingFree = isFreeSquare(row, 5) && isFreeSquare(row, 6);

		//for each square in row=row and column = i
		//columnsAttackStatus[] assign:
		//1 - under attack;		0 - not
		for (int i = 0; i < 8; i++) {
			for (auto squareUnderAttack : squaresUnderAttack) {
				if (row == squareUnderAttack.x &&  i == squareUnderAttack.y) {
					columnsAttackStatus[i] = true;
				}
			}
		}

		//1 - under attack;		0 - not
		bool queenSideAttack = columnsAttackStatus[0] || columnsAttackStatus[1] ||
			columnsAttackStatus[2] || columnsAttackStatus[3] || columnsAttackStatus[4];
		bool kingSideAttack = columnsAttackStatus[4] || columnsAttackStatus[5] ||
			columnsAttackStatus[6] || columnsAttackStatus[7];

		//is king castle possible FINAL
		if (kingPossible && kingFree && !kingSideAttack) {
			possibleMoves.push_back(Move(from, sf::Vector2i(row,4+2),MoveType::KING_CASTLE));
		}
		//is queen castle possible FINAL
		if (queenPossible && queenFree && !queenSideAttack) {
			possibleMoves.push_back(Move(from, sf::Vector2i(row, 4-2), MoveType::QUEEN_CASTLE));
		}


	}

	/*printf("afterBoard: %u\n", afterBoard.board);
	printf("currentBoard: %u\n", this->board);*/

	//afterBoard.setActivePlayer(&_activePlayer);
	//Board afterBoard = Board(&_activePlayer);

	//for each possibleMove check is it legal
	//won't be check after it
	std::vector<Move> possibleMovesFINAL;
	for (auto posMove : possibleMoves) {
		Board afterBoard = Board(this->getFEN());
		afterBoard.makeMove(posMove);
		if (!afterBoard.isCheck()) {
			possibleMovesFINAL.push_back(posMove);
		}
	}

	return possibleMovesFINAL;
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

void Board::makeMove(Move move) {
	if (MoveType::PAWN_LONG == move.moveType) {
		int targetRow = (move.from.x - move.destination.x) / 2;
		this->enPassant = sf::Vector2i(move.from.x-targetRow, move.from.y);
		//1 -> 3 3-1=2 / 2 = 1   start
		//6 -> 4 4-6=-2 / 2 = -1
	}
	else if (MoveType::EN_PASSANT == move.moveType) {
		//TODO
		board[move.from.x * 8 + move.destination.y] = nullptr;
	}
	else if(MoveType::QUEEN_CASTLE == move.moveType) {
		//TODO
		sf::Vector2i fromRook = sf::Vector2i(move.from.x, 0);
		sf::Vector2i destRook = sf::Vector2i(move.from.x, move.destination.y+1);
		makeMove(fromRook, destRook);
	}
	else if (MoveType::KING_CASTLE == move.moveType) {
		//TODO
		sf::Vector2i fromRook = sf::Vector2i(move.from.x, 7);
		sf::Vector2i destRook = sf::Vector2i(move.from.x, move.destination.y - 1);
		makeMove(fromRook, destRook);
	}
	else if (MoveType::PROMOTION == move.moveType) {
		//TODO
	}

	//REMOVE CASTLING POSSIBILITY
	if (this->at(move.from.x, move.from.y)->type == "K") {
		this->isLightKingCastlePossible = false;
		this->isLightQueenCastlePossible = false;
	}
	else if ( this->at(move.from.x, move.from.y)->type == "k") {
		this->isDarkKingCastlePossible = false;
		this->isDarkQueenCastlePossible = false;
	}
	else if (this->at(move.from.x, move.from.y)->type == "R") {
		if(move.from.y == 0)
			this->isLightQueenCastlePossible = false;
		else
			this->isLightKingCastlePossible = false;
	}
	else if (this->at(move.from.x, move.from.y)->type == "r") {
		if(move.from.y == 0)
			this->isDarkQueenCastlePossible = false;
		else
			this->isDarkKingCastlePossible = false;
	}

	if (move.moveType != MoveType::PAWN_LONG) {
		clearEnPassant();
	}
	makeMove(move.from, move.destination);
}

//get ALL squares under attack
std::vector<sf::Vector2i> Board::getSquaresUnderAttack(ColorType attackerColor, bool ignoreKing) {
	std::vector<sf::Vector2i> squares;
	bool* table = new bool[8 * 8]{0};
	// tableAt ( row*8 + col )
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (this->at(row, col) != nullptr && this->at(row,col)->getColor() == attackerColor) {
				//for each square that piece (see/is under piece's attack)
				for (auto viewedSquare : getPieceView(row, col, attackerColor, ignoreKing)) {
					table[viewedSquare.x * 8 + viewedSquare.y] = true;
				}
			}
		}
	}
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (table[row * 8 + col]) squares.push_back(sf::Vector2i(row, col));
		}
	}

	return squares;
}
//get squares that THIS PIECE attacks
std::vector<sf::Vector2i> Board::getPieceView(int row, int col, ColorType activeColor, bool ignoreKing){
	std::vector<sf::Vector2i> viewedSquares;
	if (isFreeSquare(row,col)) return viewedSquares;

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
					//if ignoreKing==true and target is king then DO NOT BREAK
					if (!(ignoreKing && isTargetKing))
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
					//if ignoreKing==true and target is king then DO NOT BREAK
					if (!(ignoreKing && isTargetKing))
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
					//if ignoreKing==true and target is king then DO NOT BREAK
					if (!(ignoreKing && isTargetKing))
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

std::string Board::getFEN() {
	//FEN POSITION
	std::string fenPosition = "";
	for (int row = 0; row < 8; row++) {
		int freeSquares = 0;
		for (int col = 0; col < 8; col++) {
			if (isFreeSquare(row, col) ){
				freeSquares++;
			}
			else {
				if (freeSquares > 0){
					fenPosition += ::std::to_string(freeSquares);
					freeSquares = 0;
				}
				fenPosition += this->at(row, col)->type;
			}
		}

		//after each row
		if (freeSquares > 0)
			fenPosition += ::std::to_string(freeSquares);
		if(row < 7)
			fenPosition += "/";
	}

	std::string fenTurn = activePlayer == ColorType::LIGHT ? "w" : "b";

	std::string fenCastling = "";
	if (this->isLightKingCastlePossible) fenCastling += "K";
	if (this->isLightQueenCastlePossible) fenCastling += "Q";
	if (this->isDarkKingCastlePossible) fenCastling += "k";
	if (this->isDarkQueenCastlePossible) fenCastling += "q";
	if (fenCastling.length() == 0) fenCastling = "-";

	//TODO
	std::string fenEnPassant = "-";
	//TODO
	std::string fenHalfMove = "0";
	//TODO
	std::string fenFullMove = "0";

	return (fenPosition + " " + fenTurn + " " + fenCastling + " " + fenEnPassant + " " + fenHalfMove + " " + fenFullMove);
}

bool Board::isCheck() {
	std::string kingType = activePlayer == ColorType::LIGHT ? "K" : "k";
	ColorType attackerColor = activePlayer == ColorType::LIGHT ? ColorType::DARK : ColorType::LIGHT;
	for (auto squareUnderAttack : this->getSquaresUnderAttack(attackerColor)) {
		if(isFreeSquare(squareUnderAttack.x, squareUnderAttack.y))continue;
		
		if (this->at(squareUnderAttack.x, squareUnderAttack.y)->type == kingType) {
			return true;
		}
	}

	return false;
}

GameStatus Board::gameStatus() {
	//1. isCheck
	//2. is there any possible move
	this->print();
	std::cout << activePlayer << "\n";

	//for each friendly piece
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			//check is there possible move
			if (isFriendlyPiece(row, col, activePlayer)) {
				if (getPossibleMoves(row, col, activePlayer).size() > 0 ) {
					return GameStatus::ON;
				}
			}
		}
	}

	if (isCheck())
		return GameStatus::MAT;
	else
		return GameStatus::PAT;
}


std::string Board::getNotation(int row, int col) {
	std::string notationStr = "";

	notationStr += std::string(1,'a'+col);
	notationStr += std::to_string(8-row);

	return notationStr;
}

sf::Vector2i Board::getCords(std::string notation) {
	int row = notation[1] - '0';
	int col = notation[0] - 'a';

	return sf::Vector2i(row, col);
}

std::vector<Move> Board::getAllPossibleMoves(ColorType activeColor) {
	std::vector<Move> allPossibleMoves;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			//if ally (friendly piece)
			if (this->at(row, col) != nullptr && this->at(row, col)->getColor() == activeColor) {
				std::vector<Move> piecePossibleMoves = this->getPossibleMoves(row, col, activeColor);
				//combine (insert into all moves) all moves and piece moves
				allPossibleMoves.insert(allPossibleMoves.begin(), piecePossibleMoves.begin(), piecePossibleMoves.end());
			}
		}
	}

	return allPossibleMoves;
}