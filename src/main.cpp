#include "screens.h"

int main()
{
	//	CUSTOM LOG FORMAT
	//	*** 20:34:49 [ info ] SFMLOrthogonalLayer.hpp:27 MapLayer(): TileLayer: 1
	spdlog::set_pattern("*** %H:%M:%S [%^ %l %$] %s:%# %!(): %v");

	//Applications variables
	std::vector<cScreen*> Screens;
	int screen = 0;

	//Window creation
	sf::RenderWindow App(sf::VideoMode(640, 480, 32), "Gyaszmat menu");

	//Mouse cursor no more visible
	App.setMouseCursorVisible(false);

	//Screens preparations
	MenuScreen s0;
	Screens.push_back(&s0);
	GameScreen s1;
	Screens.push_back(&s1);

	//Main loop
	while (screen >= 0)
	{
		screen = Screens[screen]->Run(App);
	}

	return EXIT_SUCCESS;
}