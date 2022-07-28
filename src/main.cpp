#include "Game.h"

int main()
{
	//	CUSTOM LOG FORMAT
	//	*** 20:34:49 [ info ] SFMLOrthogonalLayer.hpp:27 TileLayer(): TileLayer: 1
	spdlog::set_pattern("*** %H:%M:%S [%^ %l %$] %s:%# %!(): %v");
	SPDLOG_INFO("######################### Gyaszmat started! #########################");

	Game game;
	game.run();
	return 0;
}