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
};

