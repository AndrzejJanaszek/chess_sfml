//#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#include "Game.h"

int main() {
	srand(time(NULL));

	Game game;
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