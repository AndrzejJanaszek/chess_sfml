//#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#include "Game.h"

int main() {
	srand(time(NULL));


	Game game;
	
	while (game.isRunning()) {
		//Update
		game.update();

		//Render
		game.render();
	}

	return 0;
}