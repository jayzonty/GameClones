#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Player.h"

enum class GameState {
	Title, Game
};

int main() {
	const int screenWidth = 800;
	const int screenHeight = 600;

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Pong");

	GameClones::Pong::Game game(screenWidth, screenHeight);
	game.Init();

	int m_cursorPosition = 0;
	std::vector<std::string> m_titleOptions;
	m_titleOptions.push_back("One  Player");
	m_titleOptions.push_back("Two  Player");

	GameClones::Pong::HumanPlayer humanPlayer1(0);
	GameClones::Pong::HumanPlayer humanPlayer2(1);
	GameClones::Pong::AIPlayer aiPlayer1(0);
	GameClones::Pong::AIPlayer aiPlayer2(1);

	GameClones::Pong::Player* player1 = &aiPlayer1;
	GameClones::Pong::Player* player2 = &aiPlayer2;

	GameState gameState = GameState::Title;

	sf::Font font;
	sf::Text text;
	font.loadFromFile("Resources/Fonts/ArcadeClassic/ARCADECLASSIC.ttf");
	text.setFont(font);
	
	sf::Clock gameClock;
	while (window.isOpen()) {
		sf::Time elapsedTime = gameClock.restart();
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (gameState == GameState::Title) {
					if ((event.key.code == sf::Keyboard::W) || (event.key.code == sf::Keyboard::Up)) {
						m_cursorPosition = (m_cursorPosition - 1 + m_titleOptions.size()) % m_titleOptions.size();
					}
					else if ((event.key.code == sf::Keyboard::S) || (event.key.code == sf::Keyboard::Down)) {
						m_cursorPosition = (m_cursorPosition + 1) % m_titleOptions.size();
					}
					else if (event.key.code == sf::Keyboard::Enter) {
						if (m_cursorPosition == 0) { // One player
							player1 = &humanPlayer1;
							player2 = &aiPlayer2;
						}
						else if (m_cursorPosition == 1) { // Two player
							player1 = &humanPlayer1;
							player2 = &humanPlayer2;
						}

						game.resetGame();
						gameState = GameState::Game;
					}
				}
				else if (gameState == GameState::Game) {
					if (event.key.code == sf::Keyboard::Escape) {
						player1 = &aiPlayer1;
						player2 = &aiPlayer2;

						game.resetGame();
						gameState = GameState::Title;
					}
				}

				break;
			}
		}

		window.clear(sf::Color::Black);

		player1->Update(elapsedTime.asSeconds(), game);
		player2->Update(elapsedTime.asSeconds(), game);
		game.Update(elapsedTime.asSeconds());
		if (gameState == GameState::Title) {
			text.setCharacterSize(60);
			text.setString("PONG");
			text.setPosition((screenWidth - text.getGlobalBounds().width) / 2, 20.0f);
			window.draw(text);

			for (int i = 0; i < m_titleOptions.size(); ++i) {
				std::string str = m_titleOptions[i];

				text.setCharacterSize(30);
				text.setString(str);
				text.setPosition((screenWidth - text.getGlobalBounds().width) / 2, screenHeight * 3.0f / 4 + 40.0f * i);
				window.draw(text);

				if (m_cursorPosition == i) {
					text.setString("x ");
					text.setPosition(text.getPosition().x - 30.0f, text.getPosition().y);
					window.draw(text);
				}
			}
		}
		game.Draw(window);

		window.display();
	}

	

	return 0;
}

void HandleInput(sf::Event& inputEvent) {
	
}