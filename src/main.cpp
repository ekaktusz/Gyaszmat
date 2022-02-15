#include "Game.h"

int main()
{
	//CUSTOM LOG FORMAT
	spdlog::set_pattern("*** %H:%M:%S %s:%# %!(): %v");

	Game game;
	game.run();
	return 0;
}