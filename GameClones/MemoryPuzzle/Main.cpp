#include <cstdio>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Game.h"

#include <crtdbg.h>

#include "ListUtils.h"
#include <vector>

int main() {
	int screenWidth = 800;
	int screenHeight = 600;

	GameClones::MemoryPuzzle::Game game(screenWidth, screenHeight);
	game.Init();
	game.LoadConfiguration("Resources/Config/config.json");
	game.GenerateBoard();

	sf::Clock gameClock;
		
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Memory Puzzle");
	window.setFramerateLimit(60);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}

			game.HandleEvent(event);
		}

		sf::Time time = gameClock.restart();

		game.Update(time.asSeconds());

		window.clear(sf::Color::Black);

		game.Draw(window);

		window.display();
	}

	return 0;
}