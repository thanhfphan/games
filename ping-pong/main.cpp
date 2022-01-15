#include <iostream>
#include <SDL2/SDL.h>
#include "game.hpp"

int main(int argc, char *argv[])
{
	Game game;
	game.setup();
	while (game.is_running)
	{
		game.process_input();
		game.update();
		game.draw();
	}

	game.quit();
	
	return 0;
}