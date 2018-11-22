#include "GameState.h"

#define GAMETILE "RailDefender 3: Asteroids"

const Time dt = milliseconds(16);

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
			if (game->hasNext()) game.reset(game->getNext());
			game->update();
			game->draw(window);
		}
	}
	return 0;
}
