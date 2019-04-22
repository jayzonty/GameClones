#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Game.h"

int main() {
	const int screenWidth = 600;
	const int screenHeight = 600;

	const int cellSize = 200;

	GameClones::TicTacToe::Game game;

	sf::Font font;
	font.loadFromFile("Resources/Fonts/ArcadeClassic/ARCADECLASSIC.ttf");

	sf::Text text;
	text.setFont(font);

	sf::Texture p1Texture;
	p1Texture.loadFromFile("Resources/Textures/P1.png");
	sf::Texture p2Texture;
	p2Texture.loadFromFile("Resources/Textures/P2.png");
	sf::Texture cellTexture;
	cellTexture.loadFromFile("Resources/Textures/Cell.png");

	sf::Sprite p1Sprite(p1Texture);
	p1Sprite.setScale(cellSize * 1.0f / p1Texture.getSize().x, cellSize * 1.0f / p1Texture.getSize().y);
	sf::Sprite p2Sprite(p2Texture);
	p2Sprite.setScale(cellSize * 1.0f / p2Texture.getSize().x, cellSize * 1.0f / p2Texture.getSize().y);
	sf::Sprite cellSprite(cellTexture);
	cellSprite.setScale(cellSize * 1.0f / cellTexture.getSize().x, cellSize * 1.0f / cellTexture.getSize().y);

	sf::RectangleShape resultRect;
	resultRect.setSize(sf::Vector2f(400, 250));
	resultRect.setFillColor(sf::Color::Black);
	resultRect.setOutlineColor(sf::Color::White);
	resultRect.setOutlineThickness(10.0f);

	resultRect.setPosition((screenWidth - resultRect.getSize().x) / 2, (screenHeight - resultRect.getSize().y) / 2);
	
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Tic Tac Toe");
	while (window.isOpen()) {
		bool isGameOver = (game.GetWinner() != -1) || game.IsDrawGame();

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (!isGameOver) {
					int cellRow = event.mouseButton.y / cellSize;
					int cellColumn = event.mouseButton.x / cellSize;

					game.PlaceAt(game.GetTurn(), cellRow, cellColumn);
					isGameOver = (game.GetWinner() != -1) || game.IsDrawGame();
				}

				break;
			case sf::Event::KeyPressed:
				if (isGameOver) {
					if (event.key.code == sf::Keyboard::R) {
						game.ResetGame();
					}
					else if (event.key.code == sf::Keyboard::Escape) {
						window.close();
					}
				}
			}
		}

		window.clear(sf::Color::Black);

		// Draw game board
		for (int row = 0; row < 3; ++row) {
			for (int column = 0; column < 3; ++column) {
				// Draw the cell border
				cellSprite.setPosition(column * cellSize, row * cellSize);
				window.draw(cellSprite);

				// Draw the cell content
				int cell = game.Get(row, column);
				if (cell == 0) {
					p1Sprite.setPosition(column * cellSize, row * cellSize);
					window.draw(p1Sprite);
				}
				else if (cell == 1) {
					p2Sprite.setPosition(column * cellSize, row * cellSize);
					window.draw(p2Sprite);
				}
			}
		}

		// Draw result screen if someone won, or if it's a draw game
		if (isGameOver) {
			window.draw(resultRect);

			if (game.GetWinner() == 0) {
				text.setString("Player  1  won!");
			}
			else if (game.GetWinner() == 1) {
				text.setString("Player  2  won!");
			}
			else {
				text.setString("Draw  game!");
			}

			sf::Vector2f resultRectPos = resultRect.getPosition();
			sf::FloatRect textRect = text.getGlobalBounds();

			resultRectPos.x += (resultRect.getSize().x - textRect.width) / 2;
			resultRectPos.y += 60.0f;

			text.setPosition(resultRectPos);
			window.draw(text);

			text.setString("Press  R  to  play  again\nPress  ESC  to  exit");
			text.setPosition(resultRect.getPosition().x + (resultRect.getSize().x - text.getGlobalBounds().width) / 2, resultRectPos.y + 70.0f);
			window.draw(text);
		}

		window.display();
	}

	return 0;
}
