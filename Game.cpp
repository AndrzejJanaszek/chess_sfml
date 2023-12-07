#include "Game.h"
#include <iostream>

struct PieceName
{
	static const std::string BLACK_KING;
	static const std::string BLACK_QUEEN;
	static const std::string BLACK_ROOK;
	static const std::string BLACK_KNIGHT;
	static const std::string BLACK_BISHOP;
	static const std::string BLACK_PAWN;

	static const std::string WHITE_KING;
	static const std::string WHITE_QUEEN;
	static const std::string WHITE_ROOK;
	static const std::string WHITE_KNIGHT;
	static const std::string WHITE_BISHOP;
	static const std::string WHITE_PAWN;

	static const std::string EMPTY;
};
const std::string PieceName::BLACK_KING = "k";
const std::string PieceName::BLACK_QUEEN = "q";
const std::string PieceName::BLACK_ROOK = "r";
const std::string PieceName::BLACK_KNIGHT = "n";
const std::string PieceName::BLACK_BISHOP = "b";
const std::string PieceName::BLACK_PAWN = "p";
const std::string PieceName::WHITE_KING = "K";
const std::string PieceName::WHITE_QUEEN = "Q";
const std::string PieceName::WHITE_ROOK = "R";
const std::string PieceName::WHITE_KNIGHT = "N";
const std::string PieceName::WHITE_BISHOP = "B";
const std::string PieceName::WHITE_PAWN = "P";
const std::string PieceName::EMPTY = "-";

struct TextureName : PieceName
{
	static const std::string DARK_SQUARE;
	static const std::string LIGHT_SQUARE;
};
const std::string DARK_SQUARE = "dark_square";
const std::string LIGHT_SQUARE = "light_square";

void Game::initVariables()
{
	this->window = nullptr;

	//fonts
	if (!this->fontUbuntuMono.loadFromFile("./fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf")) {
		std::cout << "Error: Load from file Ubuntu-Mono\n";
	}

	//Textures
	std::string* piecesNames = new std::string[12]{
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
	for (int i = 0; i < 12; i++) {
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
	this->darkSquare.setFillColor(sf::Color(40, 40, 40));
	this->lightSquare = sf::RectangleShape(sf::Vector2f(100, 100));
	this->lightSquare.setFillColor(sf::Color(200, 200, 200));
}

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 800), "Title");
}

Game::Game()
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
			if ((row + col) % 2 == 0) {
				lightSquare.setPosition(col * 100, row * 100);
				this->window->draw(this->lightSquare);
			}
			else {
				darkSquare.setPosition(col * 100, row * 100);
				this->window->draw(this->darkSquare);
			}
			
			std::string pieceStr(1, this->board.at(row, col));
			if (PieceName::EMPTY.compare(pieceStr)){

				//TODO: drop shadow
				sf::Sprite pieceSprite;
				pieceSprite.setScale(100 / 256.0f, 100 / 256.0f);
				pieceSprite.setPosition(col * 100, row * 100);
				pieceSprite.setTexture(this->textures[pieceStr]);
				this->window->draw(pieceSprite);
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