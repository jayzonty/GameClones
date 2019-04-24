#include <iostream>
#include <cstdint>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Game.h"
#include "ResultModalWindow.h"

int main() {
	const int screenWidth = 600;
	const int screenHeight = 600;

	sf::Rect<int> gameAreaRect(0, 0, 600, 600);

	int rows = 20;
	int columns = 20;

	sf::Font font;
	font.loadFromFile("Resources/Fonts/DS-Digital/DS-DIGI.TTF");
	sf::Text text;
	text.setFont(font);

	sf::RectangleShape cellShape;
	sf::Vector2f cellSize;
	cellSize.x = gameAreaRect.width * 1.0f / columns;
	cellSize.y = gameAreaRect.height * 1.0f / rows;
	
	GameClones::MineSweeper::Game game;
	game.SetSize(rows, columns);
	game.GenerateBoard(50);

	GameClones::ResultModalWindow modalWindow;
	modalWindow.SetFont(font);
	modalWindow.SetSize(350, 200);
	modalWindow.SetPosition((screenWidth - 350) / 2, (screenHeight - 200) / 2);

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Minesweeper");
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (!game.DidPlayerWin() && !game.DidPlayerLose()) {
					int row = (event.mouseButton.y - gameAreaRect.top) / cellSize.y;
					int column = (event.mouseButton.x - gameAreaRect.left) / cellSize.x;
					if (event.mouseButton.button == sf::Mouse::Left) {
						game.RevealCell(row, column);

						if (game.DidPlayerWin()) {
							modalWindow.Show("YOU WON", [&game]() -> void {game.GenerateBoard(); });
						}
						else if (game.DidPlayerLose()) {
							modalWindow.Show("YOU LOST", [&game]() -> void {game.GenerateBoard(); });
						}
					}
					else if (event.mouseButton.button == sf::Mouse::Right) {
						game.ToggleCellFlag(row, column);
					}
				}

				break;
			}

			modalWindow.HandleEvent(event);
		}
		
		window.clear(sf::Color::Black);

		cellShape.setSize(cellSize);
		cellShape.setOutlineColor(sf::Color::White);
		cellShape.setOutlineThickness(2);

		text.setCharacterSize(std::min(cellSize.x, cellSize.y));
		
		for (int row = 0; row < game.GetNumRows(); ++row) {
			for (int column = 0; column < game.GetNumColumns(); ++column) {
				float x = gameAreaRect.left + column * cellSize.x;
				float y = gameAreaRect.top + row * cellSize.y;
				cellShape.setPosition(x, y);
				
				if (game.IsCellRevealed(row, column)) {
					cellShape.setFillColor(sf::Color::Black);
					window.draw(cellShape);

					if (game.IsBomb(row, column)) {
						text.setString("*");
						text.setPosition(x - text.getLocalBounds().left + (cellSize.x - text.getLocalBounds().width) / 2, y - text.getLocalBounds().top + (cellSize.y - text.getLocalBounds().height) / 2);
						window.draw(text);
					}
					else {
						int8_t cellValue = game.GetCellValue(row, column);
						if (cellValue > 0) {
							char c = (char)('0' + cellValue);
							text.setString(std::string("") + c);
							text.setPosition(x - text.getLocalBounds().left + (cellSize.x - text.getLocalBounds().width) / 2, y - text.getLocalBounds().top + (cellSize.y - text.getLocalBounds().height) / 2);
							window.draw(text);
						}
					}
				}
				else if (game.IsCellFlagged(row, column)) {
					cellShape.setFillColor(sf::Color::Blue);
					window.draw(cellShape);

					text.setString("?");
					text.setPosition(x - text.getLocalBounds().left + (cellSize.x - text.getLocalBounds().width) / 2, y - text.getLocalBounds().top + (cellSize.y - text.getLocalBounds().height) / 2);
					window.draw(text);
				}
				else {
					cellShape.setFillColor(sf::Color::Blue);
					window.draw(cellShape);
				}
			}
		}

		if (modalWindow.IsVisible()) {
			modalWindow.Draw(window);
		}

		window.display();
	}
	return 0;
}
