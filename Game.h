#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"


class Game
{
private:
	//variables
	sf::RenderWindow* window;
	sf::Font fontUbuntuMono;
	sf::Event ev;
	Board board;

	////game logic
	//ColorType activePlayer;
	//// v(-1,-1) == null
	sf::Vector2i activePiece;
	sf::Vector2i mousePosition;
	//vector of possible squares to move
	std::vector<Move> possibleMoves;

	////game objects
	//std::vector<sf::Sprite> pieceSprites;

	std::map<std::string, sf::Texture> textures;
	sf::RectangleShape darkSquare;
	sf::RectangleShape lightSquare;


	//functions
	void initVariables();
	void initWindow();
public:
	//Constructor Destructor
	Game();
	virtual ~Game();

	//Functions
	void pollEvents();
	void update();
	void render();

	bool isRunning();

	void checkGameStatus();
};

