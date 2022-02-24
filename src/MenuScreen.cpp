#include "MenuScreen.h"

MenuScreen::MenuScreen()
{
	playing = false;
	menu = 0;

	menu1.setFont(font);
	menu1.setCharacterSize(20);
	menu1.setString("Play");
	menu1.setPosition({ 280.f, 160.f });

	menu2.setFont(font);
	menu2.setCharacterSize(20);
	menu2.setString("Exit");
	menu2.setPosition({ 280.f, 220.f });

	menu3.setFont(font);
	menu3.setCharacterSize(20);
	menu3.setString("Continue");
	menu3.setPosition({ 280.f, 160.f });
}

MenuScreen::~MenuScreen()
{}

int MenuScreen::Run(sf::RenderWindow& App)
{
	if (!font.loadFromFile(
			(std::filesystem::current_path().parent_path() / "assets" / "verdanab.ttf").string()))
	{
		SPDLOG_ERROR("Error loading {}:", font.getInfo().family);
		return (-1);
	}
	SPDLOG_INFO("Font {} loaded:", font.getInfo().family);

	while (true)
	{
		//Verifying events
		while (App.pollEvent(event))
		{
			// Window closed
			if (event.type == sf::Event::Closed)
			{
				return (-1);
			}
			//Key pressed
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Up: menu = 0; break;
					case sf::Keyboard::Down: menu = 1; break;
					case sf::Keyboard::Return:
						if (menu == 0)
						{
							//Let's get play !
							playing = true;
							return (1);
						}
						else
						{
							//Let's get work...
							return (-1);
						}
						break;
					default: break;
				}
			}
		}

		if (menu == 0)
		{
			menu1.setColor(sf::Color(255, 0, 0, 255));
			menu2.setColor(sf::Color(255, 255, 255, 255));
			menu3.setColor(sf::Color(255, 0, 0, 255));
		}
		else
		{
			menu1.setColor(sf::Color(255, 255, 255, 255));
			menu2.setColor(sf::Color(255, 0, 0, 255));
			menu3.setColor(sf::Color(255, 255, 255, 255));
		}

		App.clear();
		if (playing)
		{
			App.draw(menu3);
		}
		else
		{
			App.draw(menu1);
		}
		App.draw(menu2);
		App.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}