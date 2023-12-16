#include "Game.h"
#include <iostream>

void Game::initVariables()
{
	this->window = nullptr;
	//this->board = Board(&activePlayer);

	//fonts
	if (!this->fontUbuntuMono.loadFromFile("./fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf")) {
		std::cout << "Error: Load from file Ubuntu-Mono\n";
	}

	//Textures
	const size_t _pieces_names_len_ = 12;
	std::string* piecesNames = new std::string[_pieces_names_len_]{
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
		this->textures[piecesNames[i]] = piece_texture;
	}
	delete[] piecesNames;

	//Shapes
	//textures[TextureName::DARK_SQUARE] = darkSquare;
	this->darkSquare = sf::RectangleShape(sf::Vector2f(100, 100));
	this->darkSquare.setFillColor(sf::Color(194, 126, 76));
	this->lightSquare = sf::RectangleShape(sf::Vector2f(100, 100));
	this->lightSquare.setFillColor(sf::Color(255, 255, 197));


	//play set
	//this->activePlayer = ColorType::LIGHT;
	this->activePiece = sf::Vector2i(-1,-1);

	//Set Texture to pieces
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			//draw pieces
			if (this->board.at(row, col) != nullptr) {
				std::string pieceStr = this->board.at(row, col)->type;
				this->board.at(row, col)->sprite.setTexture(this->textures[pieceStr]);
			}
		}
	}
}

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 800), "Title");
	this->window->setFramerateLimit(60);
}

Game::Game() : board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
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
				std::cout << "Kilk left\n";
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
						if (possibleMoves[i].position.x == rowCast && possibleMoves[i].position.y == colCast) {
							isMoveMade = true;

							board.makeMove(activePiece, possibleMoves[i]);
							this->activePiece = sf::Vector2i(-1,-1);
							possibleMoves.clear();
							board.swapActivePlayer();
							std::cout << "Make move!!!\n";

							board.print();
						}
					}

					if (isMoveMade == false) {
						this->activePiece = sf::Vector2i(-1,-1);
						this->possibleMoves.clear();
					}
				}
			}
		}
	}
}

void Game::update()
{
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
				lightSquare.setPosition(col * 100, row * 100);
				this->window->draw(this->lightSquare);
			}
			else {
				darkSquare.setPosition(col * 100, row * 100);
				this->window->draw(this->darkSquare);
			}
			
			for (auto move : this->possibleMoves) {
				if (move.position.x == row && move.position.y == col) {
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
			if (this->board.at(row, col) != nullptr){
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

				this->board.at(row, col)->sprite.setPosition(col * 100, row * 100);
				this->window->draw(this->board.at(row, col)->sprite);
			}
		}
	}

	//sf::Text text("rnbqkbnr\npppppppp\n--------\n--------\n--------\n--------\nPPPPPPPP\nRNBQKBNR\n"
	//	, fontUbuntuMono, 30);
	//this->window->draw(text);
	
//display
	this->window->display();
}

bool Game::isRunning()
{
	return this->window->isOpen();
}