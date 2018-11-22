#include "GameState.h"

#define GAMETILE "RailDefender 3: Asteroids"

const Time dt = milliseconds(16); // approx 60 fps

int main()
{
	Assets::load(); // load all game files
	sf::RenderWindow window(sf::VideoMode(800, 600), GAMETILE);
	shared_ptr<GameState> game(new Menu());
	Clock clock;
	clock.restart();
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		if (clock.getElapsedTime() > dt) {
			switch (game->update())
			{
			case MENU: game.reset(new Menu()); break;
			case GAME: game.reset(new Asteroids()); break;
			case EXIT: return 0;
			default: break;
			}
			game->draw(window);
			clock.restart();
		}
	}
	return 0;
}
