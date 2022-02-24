#include "MenuScreen.h"

MenuScreen::MenuScreen()
{
	this->playing = false;
	this->menu = 0;

	if (!this->font.loadFromFile(
			(std::filesystem::current_path().parent_path() / "assets" / "verdanab.ttf").string()))
	{
		SPDLOG_ERROR("Error loading {}:", this->font.getInfo().family);
	}
	SPDLOG_INFO("Font {} loaded:", this->font.getInfo().family);

	this->menu1.setFont(this->font);
	this->menu1.setCharacterSize(20);
	this->menu1.setString("Play");
	this->menu1.setPosition({ 280.f, 160.f });

	this->menu2.setFont(this->font);
	this->menu2.setCharacterSize(20);
	this->menu2.setString("Exit");
	this->menu2.setPosition({ 280.f, 220.f });

	this->menu3.setFont(this->font);
	this->menu3.setCharacterSize(20);
	this->menu3.setString("Continue");
	this->menu3.setPosition({ 280.f, 160.f });
}

MenuScreen::~MenuScreen()
{}

int MenuScreen::Run(sf::RenderWindow& App)
{
	while (true)
	{
		int returnvalue = this->update(App);
		this->render(App);
		return returnvalue;
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}

int MenuScreen::update(sf::RenderWindow& App)
{
	int returnvalue = this->processEvents(App);

	if (this->menu == 0)
	{
		this->menu1.setColor(sf::Color(255, 0, 0, 255));
		this->menu2.setColor(sf::Color(255, 255, 255, 255));
		this->menu3.setColor(sf::Color(255, 0, 0, 255));
	}

	else
	{
		this->menu1.setColor(sf::Color(255, 255, 255, 255));
		this->menu2.setColor(sf::Color(255, 0, 0, 255));
		this->menu3.setColor(sf::Color(255, 255, 255, 255));
	}

	return returnvalue;
}

int MenuScreen::render(sf::RenderWindow& App)
{
	App.clear();
	if (this->playing)
	{
		App.draw(this->menu3);
	}
	else
	{
		App.draw(this->menu1);
	}
	App.draw(this->menu2);
	App.display();

	return 0;
}

int MenuScreen::processEvents(sf::RenderWindow& App)
{
	//Verifying events
	while (App.pollEvent(this->event))
	{
		// Window closed
		if (this->event.type == sf::Event::Closed)
		{
			return (-1);
		}
		//Key pressed
		if (this->event.type == sf::Event::KeyPressed)
		{
			switch (this->event.key.code)
			{
				case sf::Keyboard::Up: this->menu = 0; break;
				case sf::Keyboard::Down: this->menu = 1; break;
				case sf::Keyboard::Return:
					if (this->menu == 0)
					{
						//Let's get play !
						this->playing = true;
						SPDLOG_INFO("Pressed PLAY / CONTINUE");

						return (1);
					}
					else
					{
						//Let's get work...
						SPDLOG_INFO("Pressed EXIT");

						return (-1);
					}
					break;
				default: break;
			}
		}
	}

	return 0;
}