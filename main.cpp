//#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.h"

int main() {
	Game game;

	//sf::RenderWindow window(sf::VideoMode(800,800), "Title");

	/*sf::Font fontUbuntu;
	if (!fontUbuntu.loadFromFile("./fonts/Ubuntu/Ubuntu-Regular.ttf")) {
		std::cout << "Error: Load from file Ubuntu-Regular\n";
	}*/

	while (game.isRunning()) {
		//Update
		game.update();

		//Render
		game.render();
	}

	return 0;
}