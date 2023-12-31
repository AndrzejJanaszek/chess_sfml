#include <iostream>
#include "Game.h"
#include "BotMachina.h"

void Game::initVariables()
{
	this->endStatus = GameStatus::ON;
	this->AI_ON = false;
	this->window = nullptr;
	//this->board = Board(&activePlayer);

	//fonts
	if (!this->fontUbuntuMono.loadFromFile("./fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf")) {
		std::cout << "Error: Load from file Ubuntu-Mono\n";
	}

	//Textures
	const size_t _pieces_names_len_ = 12;
	char* piecesNames = new char[_pieces_names_len_]{
		PieceName::BLACK_KING,
		PieceName::BLACK_QUEEN,
		PieceName::BLACK_ROOK,
		PieceName::BLACK_KNIGHT,
		PieceName::BLACK_BISHOP,
		PieceName::BLACK_PAWN,

		PieceName::WHITE_KING,
		PieceName::WHITE_QUEEN,
		PieceName::WHITE_ROOK,
		PieceName::WHITE_KNIGHT,
		PieceName::WHITE_BISHOP,
		PieceName::WHITE_PAWN
	};
	for (int i = 0; i < _pieces_names_len_; i++) {
		sf::Texture piece_texture;
		piece_texture.setSmooth(true);
		const std::string s1 = "./pieces/";
		const std::string s2 = ".png";
		if (i < 6) {
			if (!piece_texture.loadFromFile(s1 + "black/" + piecesNames[i] + s2)) {
				std::cout << "Error: Failed to load black "+ piecesNames[i] + s2;
			}
		}else {
			if (!piece_texture.loadFromFile(s1 + "white/" + piecesNames[i] + s2)) {
				std::cout << "Error: Failed to load white " + piecesNames[i] + s2;
			}
		}
		this->textures[std::string(1, piecesNames[i])] = piece_texture;
	}
	delete[] piecesNames;

	//square shadow texture
	sf::Texture square_shadow_texture;
	
	if (!square_shadow_texture.loadFromFile("./textures/square_shadow100.png")) {
		std::cout << "Error: Failed to load black ./textures/square_shadow100.png";
	}
	this->textures["square_shadow"] = square_shadow_texture;

	//Shapes
	//textures[TextureName::DARK_SQUARE] = darkSquare;
	this->darkSquare = sf::RectangleShape(sf::Vector2f(100, 100));
	this->darkSquare.setFillColor(sf::Color(194, 126, 76));
	this->lightSquare = sf::RectangleShape(sf::Vector2f(100, 100));
	this->lightSquare.setFillColor(sf::Color(255, 255, 197));


	//play set
	//this->activePlayer = ColorType::LIGHT;
	this->activePiece = sf::Vector2i(-1,-1);

}

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 800), "Title");
	//this->window->setFramerateLimit(60);
}

//Game::Game() : board("8/8/3k4/8/8/3K4/8/8 w - - 0 1")
//Game::Game() : board("8/p7/3k4/8/8/3K4/8/8 w - - 0 1")
//king of \/
//Game::Game() : board("rnbq1bnr/pppppppp/7k/8/8/8/PPPPPPPP/RNBQKBNR w KQ - 0 1")
//2
//Game::Game() : board("rnbqbnr1/pppppppp/7k/8/8/8/PPPPPPPP/RNBQKBNR w KQ - 0 1")
//3
Game::Game() : board("rnbqbnrk/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQq - 0 1")
//Game::Game() : board("bb2k3/3np1p1/2p2pn1/1p6/7P/2N1PNP1/1PPP1P2/2B1KB2 w - - 0 1")
//Game::Game() : board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{
	initVariables();
	initWindow();
}

Game::~Game()
{
	delete this->window;
}

//Functions
void Game::pollEvents() {
	while (this->window->pollEvent(this->ev)) {
		if (this->ev.type == sf::Event::Closed) {
			this->window->close();
		}
		if (this->ev.type == sf::Event::MouseButtonPressed) {
			//klikniecie bierki
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				this->mousePosition = sf::Mouse::getPosition( *this->window );
				
				// position / square size
				int rowCast = mousePosition.y / 100;
				int colCast = mousePosition.x / 100;

				if (activePiece.x < 0) {
					if (board.isFriendlyPiece(rowCast, colCast, board.getActivePlayer())) {
						activePiece = sf::Vector2i(rowCast, colCast);
						this->possibleMoves = board.getPossibleMoves(rowCast, colCast, board.getActivePlayer());
					}
				}
				else {
					bool isMoveMade = false;
					for (int i = 0; i < this->possibleMoves.size(); i++){
						//Made a possible move!!!
						if (possibleMoves[i].destination.x == rowCast && possibleMoves[i].destination.y == colCast) {
							isMoveMade = true;

							board.makeMove(possibleMoves[i]);
							this->activePiece = sf::Vector2i(-1,-1);
							possibleMoves.clear();

							this->checkGameStatus();

							if (this->AI_ON) {
								//BOT MACHINA MOVE
								BotMachina botMachina(board.getActivePlayer());
								std::cout << "Bot\n";
								//Move botMove = botMachina.getMove(this->board.getFEN());
								Board boardForBot(this->board.getFEN());
								Move botMove = botMachina.depthSearch(boardForBot, 1);
								if (!botMove.isEmpty()) {
									this->board.makeMove(botMove);
								}

								this->checkGameStatus();
							}
						}
					}

					if (isMoveMade == false) {
						this->activePiece = sf::Vector2i(-1,-1);
						this->possibleMoves.clear();
					}
				}
			}

		}
		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ) {
			this->AI_ON = !this->AI_ON;
		}
	}
}

void Game::update()
{
	//std::cout << this->board.getActivePlayer() << std::endl;
	this->pollEvents();
}

void Game::render()
{
//clear
	this->window->clear();
//draw
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			//draw board (dark and ligth squares)
			if ((row + col) % 2 == 0) {
				//light
				lightSquare.setPosition(col * 100, row * 100);
				this->window->draw(this->lightSquare);
			}
			else {
				//dark
				darkSquare.setPosition(col * 100, row * 100);
				this->window->draw(this->darkSquare);

				//shadow
				sf::Sprite squareShadowSprite;
				squareShadowSprite.setScale(1,1);
				squareShadowSprite.setPosition(col * 100, row * 100);
				squareShadowSprite.setTexture(this->textures[TextureName::SQUARE_SHADOW]);
				squareShadowSprite.setColor(sf::Color(255,255,255,80));
				this->window->draw(squareShadowSprite);
			}
			
			for (auto move : this->possibleMoves) {
				if (move.destination.x == row && move.destination.y == col) {
					if ((row + col) % 2 == 0) {
						//light
						sf::RectangleShape squareShape = sf::RectangleShape(sf::Vector2f(100, 100));
						squareShape.setPosition(col * 100, row * 100);
						squareShape.setFillColor(sf::Color(200, 70, 50));
						this->window->draw(squareShape);
					}
					else {
						//dark
						sf::RectangleShape squareShape = sf::RectangleShape(sf::Vector2f(100, 100));
						squareShape.setPosition(col * 100, row * 100);
						squareShape.setFillColor(sf::Color(160, 20, 20));
						this->window->draw(squareShape);
					}
				}
			}

			//draw pieces
			if (this->board.at(row, col) != PieceName::EMPTY){
				/*std::string pieceStr = this->board.at(row, col)->type;*/
				
				/*sf::Sprite pieceSprite;
				pieceSprite.setScale(100 / 256.0f, 100 / 256.0f);
				pieceSprite.setPosition(col * 100, row * 100);
				pieceSprite.setTexture(this->textures[pieceStr]);*/
				//this->board.at(row, col)->sprite.setTexture(this->textures[pieceStr]);
				if (activePiece.x == row && activePiece.y == col) {
					sf::RectangleShape squareShape = sf::RectangleShape(sf::Vector2f(100,100));
					squareShape.setPosition(col * 100, row * 100);
					squareShape.setFillColor(sf::Color(100, 130, 20));
					this->window->draw(squareShape);
				}

				sf::Sprite pieceSprite;
				pieceSprite.setPosition(col * 100, row * 100);
				pieceSprite.setTexture(this->textures[std::string(1, this->board.at(row, col))]);
				pieceSprite.setScale(100 / 256.0f, 100 / 256.0f);
				this->window->draw(pieceSprite);
			}
		}
	}

	//sf::Text text("rnbqkbnr\npppppppp\n--------\n--------\n--------\n--------\nPPPPPPPP\nRNBQKBNR\n"
	//	, fontUbuntuMono, 30);
	//this->window->draw(text);
	
	if (this->endStatus == GameStatus::MAT) {
		sf::RectangleShape endShape(sf::Vector2f(800, 800));
		sf::Color winnerColor = this->board.getActivePlayer() == ColorType::LIGHT ? sf::Color(20, 20, 20) : sf::Color(240, 240, 240);
		endShape.setFillColor(winnerColor);
		endShape.setPosition(0, 0);

		this->window->draw(endShape);
	}
	else if (this->endStatus == GameStatus::PAT) {
		sf::RectangleShape endShape(sf::Vector2f(800, 800));
		endShape.setFillColor(sf::Color(120, 120, 120));
		endShape.setPosition(0, 0);

		this->window->draw(endShape);
	}
		
//display
	this->window->display();
}

bool Game::isRunning()
{
	return this->window->isOpen();
}

void Game::checkGameStatus() {
	GameStatus gs = this->board.gameStatus();

	if (gs == GameStatus::ON) {
		//nothin
		//this->endStatus = GameStatus::ON;
	}
	else if (gs == GameStatus::MAT) {
		std::string winner = this->board.getActivePlayer() == ColorType::LIGHT ? "Czarny" : "Bia�y";
		//std::cout << "KONIEC GRY\nMAT\nWygra�: " << winner << " kolor" << std::endl;
		
		this->endStatus = GameStatus::MAT;
	}
	else if (gs == GameStatus::PAT) {
		std::string winner = this->board.getActivePlayer() == ColorType::LIGHT ? "Czarny" : "Bia�y";
		//std::cout << "KONIEC GRY\nPAT\n";

		this->endStatus = GameStatus::PAT;
	}
}