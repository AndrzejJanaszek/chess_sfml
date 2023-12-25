#include "Board.h"
#include <string>
#include <iostream>
#include <memory>
#include "Constants.h"

Board::Board(std::string fen)
{
	//this->fen = fen;
	this->fenHistory.push_back(fen);
	this->initVariables();

	this->initBoard();
}

Board::~Board()
{
	delete[] board;
}

void Board::initVariables() {
	this->board = new char[8*8];
	this->enPassant = sf::Vector2i(-1, -1);

	this->isDarkKingCastlePossible = false;
	this->isDarkQueenCastlePossible = false;
	this->isLightKingCastlePossible = false;
	this->isLightQueenCastlePossible = false;
}

void Board::initBoard() {
	//if empty: -
	//std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	//std::string fen = "1r2kbnr/pp1ppppp/2p2q2/1NP5/B2b4/1n3NQ1/PP1P1PPP/R3KB1R w KQk - 0 1";
	//std::string fen = "3q4/8/6P1/PK1N1k2/PP1p4/p5np/4pP1p/5r2 w - - 0 1";<

	std::vector<std::string> fenSplited;

	//split fen by ' ' into 6 strings
	std::string chunkStr = "";
	std::string fen = this->fenHistory.back();
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
				board[j] = PieceName::EMPTY;
				j++;
			}
			continue;
		}

		//piece
		board[j] = fenPosition[i];
		int row = j / 8;
		int col = j % 8;
		//board[j]->sprite.setPosition(100 * col, 100 * row);
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

char Board::at(int row, int col) {
	return board[row * 8 + col];
}

void Board::setAt(int row, int col, char type){
	board[row * 8 + col] = type;
}

ColorType Board::getPieceColorAt(int row, int col) {
	return this->getPieceColor(this->at(row, col));
}

ColorType Board::getPieceColor(char type) {
	if (65 <= type && type <= 90)
		return ColorType::LIGHT;
	return ColorType::DARK;
}

char Board::getPieceCapsTypeAt(int row, int col) {
	return this->getPieceCapsType(this->at(row, col));
}

char Board::getPieceCapsType(char type) {
	if (65 <= type && type <= 90) {
		//white
		return type;
	}
	return type - 32;
}

void Board::print() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->board[i * 8 + j] != PieceName::EMPTY) {
				std::cout << this->board[i*8 + j];
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

	if (this->getPieceColorAt(row, col) == activeColor)
		return true;
	return false;
}

bool Board::isEnemyPiece(int row, int col, ColorType activeColor) {
	if (this->isFreeSquare(row, col)) return false;

	if (this->getPieceColorAt(row, col) != activeColor)
		return true;
	return false;
}

bool Board::isFreeSquare(int row, int col) {
	if (this->at(row, col) == PieceName::EMPTY) return true;

	return false;
}

//int row, int col => V2::from
std::vector<Move> Board::getPossibleMoves(int row, int col, ColorType activeColor) {
	std::vector<Move> possibleMoves;
	if (isFreeSquare(row,col)) return possibleMoves;

	sf::Vector2i from(row, col);
	char pieceType = this->getPieceCapsTypeAt(row, col);
	//pawn
	if (pieceType == PieceName::WHITE_PAWN) {
		int firstPawnRow = this->activePlayer == ColorType::LIGHT ? 6 : 1;
		int promotionRow = 7-firstPawnRow;
		//for pawn
		int colorDirectionMultiplier = activeColor == ColorType::LIGHT ? -1 : 1;
		sf::Vector2i attack1	= sf::Vector2i(row + 1 * colorDirectionMultiplier, col - 1);
		sf::Vector2i attack2	= sf::Vector2i(row + 1 * colorDirectionMultiplier, col + 1);
		sf::Vector2i move1		= sf::Vector2i(row + 1 * colorDirectionMultiplier, col);
		sf::Vector2i move2		= sf::Vector2i(row + 2 * colorDirectionMultiplier, col);

		//attacks
		//if onBoard AND isEnemy
		if ( this->isMoveOnBoard(attack1.x, attack1.y) ) {
			if (enPassant.x == attack1.x && enPassant.y == attack1.y) {
				possibleMoves.push_back(Move(from, attack1, MoveType::EN_PASSANT) );
			}else if ( isEnemyPiece(attack1.x, attack1.y, activeColor)) {
				if (row == promotionRow)
					possibleMoves.push_back(Move(from, attack1, MoveType::PROMOTION));
				else
					possibleMoves.push_back(Move(from, attack1, MoveType::TAKE) );
			}
		}
		if (this->isMoveOnBoard(attack2.x, attack2.y)) {
			if (enPassant.x == attack2.x && enPassant.y == attack2.y) {
				possibleMoves.push_back(Move(from, attack2, MoveType::EN_PASSANT));
			}else if (isEnemyPiece(attack2.x, attack2.y, activeColor)) {
				if (row == promotionRow)
					possibleMoves.push_back(Move(from, attack2, MoveType::PROMOTION));
				else
					possibleMoves.push_back(Move(from, attack2, MoveType::TAKE));
			}
		}

		//move1
		//if onBoard AND isFreeSquare
		if (this->isMoveOnBoard(move1.x, move1.y) && isFreeSquare(move1.x, move1.y)) {
			if (row == promotionRow)
				possibleMoves.push_back(Move(from, move1, MoveType::PROMOTION));
			else
				possibleMoves.push_back(Move(from, move1, MoveType::NORMAL) );

			//move2 PAWN_LONG
			if (firstPawnRow == row) {
				if (this->isMoveOnBoard(move2.x, move2.y) && isFreeSquare(move2.x, move2.y)) {
					possibleMoves.push_back(Move(from, move2, MoveType::PAWN_LONG));
				}
			}
		}
	}

	if (pieceType == PieceName::WHITE_KNIGHT) {

		for (auto moveT : MovesPatterns::KNIGHT) {
			moveT.x += row;
			moveT.y += col;
			//if not on board continue
			if (!isMoveOnBoard(moveT.x, moveT.y)) continue;

			if (isFreeSquare(moveT.x, moveT.y)) {
				possibleMoves.push_back(Move(from, moveT, MoveType::NORMAL));
			}else if (isEnemyPiece(moveT.x, moveT.y, activeColor) ) {
				possibleMoves.push_back(Move(from, moveT, MoveType::TAKE));
			}
		}
	}

	if (pieceType == PieceName::WHITE_ROOK) {

		//for each direction
		for (auto &mDir : MovesPatterns::ROOK) {
			sf::Vector2i move = sf::Vector2i(row + mDir.x, col + mDir.y);

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
				move.x += mDir.x;
				move.y += mDir.y;
			}
		}
	}

	if (pieceType == PieceName::WHITE_BISHOP) {
		//for each direction
		for (auto &mDir : MovesPatterns::BISHOP) {
			sf::Vector2i move = sf::Vector2i(row + mDir.x, col + mDir.y);

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
				move.x += mDir.x;
				move.y += mDir.y;
			}
		}
	}

	if (pieceType == PieceName::WHITE_QUEEN) {
		//for each direction
		for (auto &mDir : MovesPatterns::QUEEN) {
			sf::Vector2i move = sf::Vector2i(row + mDir.x, col + mDir.y);

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
				move.x += mDir.x;
				move.y += mDir.y;
			}
		}
	}

	if (pieceType == PieceName::WHITE_KING) {
		ColorType enemyColor = activeColor == ColorType::LIGHT ? ColorType::DARK : ColorType::LIGHT;
		std::unique_ptr<bool[]> squaresUnderAttack = getSquaresUnderAttack(enemyColor);

		for (auto move : MovesPatterns::KING) {
			move.x += row;
			move.y += col;
			if (!isMoveOnBoard(move.x, move.y)) continue;

			//is square under attack => continue/skip_move
			if (squaresUnderAttack[move.x * 8 + move.y])
				continue;

			//if not underAttack and is not friendly piece
			if (!isFriendlyPiece(move.x, move.y, activeColor)) {
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

		//QUEEN col [0,1,2,3]
		//KING col	[5,6,7]
		// + is king under attack [4]

		//free squres check
		bool queenFree = isFreeSquare(row, 1) && isFreeSquare(row, 2) && isFreeSquare(row, 3);
		bool kingFree = isFreeSquare(row, 5) && isFreeSquare(row, 6);

		//1 - under attack;		0 - not
		bool queenSideAttack = squaresUnderAttack[8 * row + 0] || squaresUnderAttack[8 * row + 1] ||
			squaresUnderAttack[8 * row + 2] || squaresUnderAttack[8 * row + 3] || squaresUnderAttack[8 * row + 4];
		bool kingSideAttack = squaresUnderAttack[8 * row + 4] || squaresUnderAttack[8 * row + 5] ||
			squaresUnderAttack[8 * row + 6] || squaresUnderAttack[8 * row + 7];

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
	//Board afterBoard(this->getFEN());
	for (auto &posMove : possibleMoves) {
		this->makeMove(posMove);
		if (!this->isCheckReversed()) {
			possibleMovesFINAL.push_back(posMove);
		}
		this->undoMove();
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
	board[from.x * 8 + from.y] = PieceName::EMPTY;
	this->swapActivePlayer();

	this->fenHistory.push_back(this->getFEN());
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
		board[move.from.x * 8 + move.destination.y] = PieceName::EMPTY;
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
		if (getPieceColorAt(move.from.x, move.from.y) == LIGHT) {
			this->setAt(move.from.x, move.from.y, 'Q');
		}
		else {
			this->setAt(move.from.x, move.from.y, 'q');
		}
	}

	//REMOVE CASTLING POSSIBILITY
	//Piece* ppp = this->at(move.from.x, move.from.y);
	if (this->at(move.from.x, move.from.y) == 'K') {
		this->isLightKingCastlePossible = false;
		this->isLightQueenCastlePossible = false;
	}
	else if (this->at(move.from.x, move.from.y) == 'k') {
		this->isDarkKingCastlePossible = false;
		this->isDarkQueenCastlePossible = false;
	}
	else if (this->at(move.from.x, move.from.y) == 'R') {
		if(move.from.y == 0)
			this->isLightQueenCastlePossible = false;
		else
			this->isLightKingCastlePossible = false;
	}
	else if (this->at(move.from.x, move.from.y) == 'r') {
		if(move.from.y == 0)
			this->isDarkQueenCastlePossible = false;
		else
			this->isDarkKingCastlePossible = false;
	}

	if (move.moveType != MoveType::PAWN_LONG) {
		clearEnPassant();
	}
	makeMove(move.from, move.destination);
	//this->print();
}

//get ALL squares under attack
std::unique_ptr<bool[]> Board::getSquaresUnderAttack(ColorType attackerColor) {
	std::unique_ptr<bool[]> table( new bool[8 * 8] {0} );
	// tableAt ( row*8 + col )
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			//foreach attacker
			if (this->at(row, col) != PieceName::EMPTY && this->getPieceColorAt(row, col) == attackerColor) {
				std::cout << this->at(row, col) << "\n";
				//auto squaresUnderCurrentPieceAtatck = std::move(getPieceView(row, col, attackerColor));
				std::unique_ptr<bool[]> squaresUnderCurrentPieceAtatck = getPieceView(row, col, attackerColor);
				/*
				print
				for (int _row = 0; _row < 8; _row++) {
					for (int _col = 0; _col < 8; _col++) {
						std::cout << squaresUnderCurrentPieceAtatck[_row * 8 + _col];
					}
					std::cout << "\n";
				}
				std::cout << "\n";*/

				//if square under attack set table[i] = 1
				for (int i = 0; i < 64; i++) {
					table[i] = table[i] || squaresUnderCurrentPieceAtatck[i];
				}
				
			}
		}
	}

	return table;
}
//get squares that THIS PIECE attacks
std::unique_ptr<bool[]> Board::getPieceView(int row, int col, ColorType activeColor){
	std::unique_ptr<bool[]> arr(new bool[8 * 8] {0});
	//std::unique_ptr<bool[]> arr(new bool[8 * 8] {0});
	
	if (isFreeSquare(row,col)) return arr;

	char pieceType = getPieceCapsTypeAt(row, col);
	//pawn
	if (pieceType == PieceName::WHITE_PAWN) {
		//for pawn
		int colorDirectionMultiplier = activeColor == ColorType::LIGHT ? -1 : 1;
		sf::Vector2i attack1 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col - 1);
		sf::Vector2i attack2 = sf::Vector2i(row + 1 * colorDirectionMultiplier, col + 1);

		//attacks
		//if onBoard AND isEnemy
		if (this->isMoveOnBoard(attack1.x, attack1.y)) {
			if (isEnemyPiece(attack1.x, attack1.y, activeColor) || (enPassant.x == attack1.x && enPassant.y == attack1.y)) {
				arr[8 * attack1.x + attack1.y] = 1;
			}
		}
		if (this->isMoveOnBoard(attack2.x, attack2.y)) {
			if (isEnemyPiece(attack2.x, attack2.y, activeColor) || (enPassant.x == attack2.x && enPassant.y == attack2.y)) {
				arr[8 * attack2.x + attack2.y] = 1;
			}
		}
	}

	else if (pieceType == PieceName::WHITE_KNIGHT) {
		for (auto moveT : MovesPatterns::KNIGHT) {
			moveT.x += row;
			moveT.y += col;
			//if not on board continue
			if (!isMoveOnBoard(moveT.x, moveT.y)) continue;

			arr[8 * moveT.x + moveT.y] = 1;
		}
	}

	else if (pieceType == PieceName::WHITE_ROOK) {
		//for each direction
		for (auto &mDir : MovesPatterns::ROOK) {
			sf::Vector2i move = sf::Vector2i(row + mDir.x, col + mDir.y);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					arr[8 * move.x + move.y] = 1;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor) || this->isFriendlyPiece(move.x, move.y, activeColor)) {
					arr[8 * move.x + move.y] = 1;
					break;
					
					////if kingFuture = true skip KING
					//bool isTargetKing = (this->at(move.x, move.y) == PieceName::WHITE_KING || this->at(move.x, move.y) == PieceName::BLACK_KING);
					////if ignoreKing==true and target is king then DO NOT BREAK
					//if (!(ignoreKing && isTargetKing))
					//	break;
				}
				move.x += mDir.x;
				move.y += mDir.y;
			}
		}
	}

	else if (pieceType == PieceName::WHITE_BISHOP) {
		//for each direction
		for (auto mDir : MovesPatterns::BISHOP) {
			sf::Vector2i move = sf::Vector2i(row + mDir.x, col + mDir.y);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					arr[8 * move.x + move.y] = 1;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor) || this->isFriendlyPiece(move.x, move.y, activeColor)) {
					arr[8 * move.x + move.y] = 1;
					break;
					////if kingFuture = true skip KING
					//bool isTargetKing = (this->at(move.x, move.y) == PieceName::WHITE_KING || this->at(move.x, move.y) == PieceName::BLACK_KING);
					////if ignoreKing==true and target is king then DO NOT BREAK
					//if (!(ignoreKing && isTargetKing))
					//	break;
				}

				move.x += mDir.x;
				move.y += mDir.y;
			}
		}
	}

	else if (pieceType == PieceName::WHITE_QUEEN) {

		//for each direction
		for (auto mDir : MovesPatterns::QUEEN) {
			sf::Vector2i move = sf::Vector2i(row + mDir.x, col + mDir.y);

			while (this->isMoveOnBoard(move.x, move.y)) {
				if (this->isFreeSquare(move.x, move.y)) {
					arr[8 * move.x + move.y] = 1;
				}
				else if (this->isEnemyPiece(move.x, move.y, activeColor) || this->isFriendlyPiece(move.x, move.y, activeColor)) {
					arr[8 * move.x + move.y] = 1;
					break;
					////if kingFuture = true skip KING
					//bool isTargetKing = (this->at(move.x, move.y) == PieceName::WHITE_KING || this->at(move.x, move.y) == PieceName::BLACK_KING);
					////if ignoreKing==true and target is king then DO NOT BREAK
					//if (!(ignoreKing && isTargetKing))
					//	break;
				}
				move.x += mDir.x;
				move.y += mDir.y;
			}
		}
	}

	else if (pieceType == PieceName::WHITE_KING) {
		for (auto move : MovesPatterns::KING) {
			move.x += row;
			move.y += col;
			arr[8 * move.x + move.y] = 1;
		}
	}

	return arr;
}

//std::unique_ptr<bool[]> Board::getSquaresUnderAttack(ColorType attackerColor) {
//	return getSquaresUnderAttack(attackerColor, false);
//}
//std::unique_ptr<bool[]> Board::getPieceView(int row, int col, ColorType activeColor) {
//	return getPieceView(row, col, activeColor, false);
//}

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
				fenPosition += this->at(row, col);
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
	char kingType = activePlayer == ColorType::LIGHT ? PieceName::WHITE_KING : PieceName::BLACK_KING;
	ColorType attackerColor = activePlayer == ColorType::LIGHT ? ColorType::DARK : ColorType::LIGHT;

	std::unique_ptr<bool[]> squaresUnderAttack = this->getSquaresUnderAttack(attackerColor);

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (this->at(row, col) == kingType && squaresUnderAttack[8 * row + col]) {
				return true;
			}
		}
	}

	return false;
}
//checkKingColor - color of king that will be checked/attacked
bool Board::isCheckReversed() {
	char kingType = activePlayer == ColorType::LIGHT ? PieceName::BLACK_KING : PieceName::WHITE_KING;
	ColorType attackerColor = activePlayer == ColorType::LIGHT ? ColorType::LIGHT : ColorType::DARK;

	std::unique_ptr<bool[]> squaresUnderAttack = this->getSquaresUnderAttack(attackerColor);

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (this->at(row, col) == kingType && squaresUnderAttack[8 * row + col]) {
				return true;
			}
		}
	}

	return false;
}

GameStatus Board::gameStatus() {
	//1. isCheck
	//2. is there any possible move
	
	int piece = 0;
	int BN = 0;

	std::vector<Move> allPosMoves = getAllPossibleMoves();

	if (allPosMoves.size() == 0) {
		if (isCheck())
			return GameStatus::MAT;
		else
			return GameStatus::PAT;
	}



	//for each friendly piece
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (this->isFreeSquare(row, col))continue;

			if (getPieceCapsTypeAt(row, col) != PieceName::WHITE_KING) {
				piece++;
				if (piece > 1)
					return GameStatus::ON;
			}

			//check is there possible move

			//if (this->at(row, col)->getCapitalType() != "K") {
			/*	piece++;
				if(piece > 1)
					return GameStatus::ON;

				if (this->at(row, col)->getCapitalType() == "B" || this->at(row, col)->getCapitalType() == "N") {
					BN++;
				}
				else if (isFriendlyPiece(row, col, activePlayer)) {
					if (getPossibleMoves(row, col, activePlayer).size() > 0) {
						return GameStatus::ON;
					}
				}
			}*/
		}
	}

	//figur¹ inn¹ ni¿ B/N mo¿na zmatowaæ
	if (BN == 1 || piece == 0) {
		return GameStatus::PAT;
	}

	return GameStatus::ON;
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

std::vector<Move> Board::getAllPossibleMoves() {
	std::vector<Move> allPossibleMoves;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			//if ally (friendly piece)
			if (this->at(row, col) != PieceName::EMPTY && getPieceColorAt(row, col) == this->activePlayer) {
				std::vector<Move> piecePossibleMoves = this->getPossibleMoves(row, col, this->activePlayer);
				//combine (insert into all moves) all moves and piece moves
				allPossibleMoves.insert(allPossibleMoves.begin(), piecePossibleMoves.begin(), piecePossibleMoves.end());
			}
		}
	}

	return allPossibleMoves;
}

void Board::undoMove() {
	if (this->fenHistory.size() > 1) {
		this->fenHistory.pop_back();
		this->initBoard();
	}
}