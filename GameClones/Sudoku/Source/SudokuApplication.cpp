#include "SudokuApplication.h"

#include <functional>
#include <iostream>

#include "Common/Color.h"
#include "Common/FileUtils.h"
#include "Common/Input.h"
#include "Common/Math.h"
#include "Common/Window.h"
#include "Common/WindowManager.h"

namespace GameClones
{
	namespace Sudoku
	{
		SudokuApplication::SudokuApplication()
			: BaseApplication("Sudoku")
			, m_random()
			, m_renderTarget()
			, m_buttonsFont()
			, m_newGameButton()
			, m_checkSolutionButton()
			, m_resultText()
			, m_sudokuGrid()
			, m_sudokuGridChecker()
			, m_sudokuGridRenderer()
			, m_selectedRow(0)
			, m_selectedColumn(0)
			, m_puzzleStrings()
		{
		}

		SudokuApplication::~SudokuApplication()
		{
		}

		void SudokuApplication::SetupWindow(Common::Window* window)
		{
			window->SetSize(1200, 900);
			window->SetTitle("Sudoku");

			m_renderTarget.SetSize(1200, 900);
		}

		void SudokuApplication::Init()
		{
			ReadPuzzleStrings("Resources/Data/puzzles.txt", m_puzzleStrings);

			if (m_puzzleStrings.size() > 0)
			{
				LoadGridFromPuzzleString(m_puzzleStrings[0], m_sudokuGrid);
			}

			m_buttonsFont.Load("Resources/Fonts/DS-Digital/DS-DIGIB.TTF");

			m_newGameButton.SetFont(m_buttonsFont);
			m_newGameButton.SetPosition(950.0f, 500.0f);
			m_newGameButton.SetSize(200.0f, 100.0f);
			m_newGameButton.SetText("New Game");
			m_newGameButton.AddClickListener(std::bind(&SudokuApplication::NewGame, this));

			m_checkSolutionButton.SetFont(m_buttonsFont);
			m_checkSolutionButton.SetPosition(950.0f, 380.0f);
			m_checkSolutionButton.SetSize(200.0f, 100.0f);
			m_checkSolutionButton.SetText("Check Solution");
			m_checkSolutionButton.AddClickListener(std::bind(&SudokuApplication::CheckSolution, this));

			m_resultText.SetFont(m_buttonsFont);
			m_resultText.SetPosition(950.0f, 260.0f);
			m_resultText.SetHorizontalAlignment(Common::Text::HorizontalAlignment::CENTER);
			m_resultText.SetString("");
			m_resultText.SetColor(Common::Color::White);
		}

		void SudokuApplication::Update(float deltaTime)
		{
			size_t numRows = 0, numColumns = 0;
			m_sudokuGrid.GetSize(numRows, numColumns);

			if (Common::Input::IsPressed(Common::Input::LEFT_MOUSE))
			{
				int mouseX = 0, mouseY = 0;
				Common::Input::GetMousePosition(&mouseX, &mouseY);

				if (m_newGameButton.GetBounds().ContainsPoint(mouseX, 900 - mouseY))
				{
					m_newGameButton.Click();
				}
				else if (m_checkSolutionButton.GetBounds().ContainsPoint(mouseX, 900 - mouseY))
				{
					m_checkSolutionButton.Click();
				}
				else
				{
					m_selectedRow = static_cast<size_t>(mouseY / GetCellSize());
					m_selectedColumn = static_cast<size_t>(mouseX / GetCellSize());
				}
			}
			else if (Common::Input::IsPressed(Common::Input::LEFT))
			{
				m_selectedColumn = Common::Math::Max<size_t>(m_selectedColumn - 1, 0);
			}
			else if (Common::Input::IsPressed(Common::Input::RIGHT))
			{
				m_selectedColumn = Common::Math::Min<size_t>(m_selectedColumn + 1, numColumns - 1);
			}
			else if (Common::Input::IsPressed(Common::Input::UP))
			{
				m_selectedRow = Common::Math::Max<size_t>(m_selectedRow - 1, 0);
			}
			else if (Common::Input::IsPressed(Common::Input::DOWN))
			{
				m_selectedRow = Common::Math::Min<size_t>(m_selectedRow + 1, numRows - 1);
			}
			m_sudokuGridRenderer.SetSelectedCell(m_selectedRow, m_selectedColumn);

			if (m_sudokuGrid.IsCellModifiable(m_selectedRow, m_selectedColumn))
			{
				// TODO: Should have a better way of doing this
				if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA1, Common::Input::Key::KEYPAD1 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '1');
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA2, Common::Input::Key::KEYPAD2 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '2');
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA3, Common::Input::Key::KEYPAD3 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '3');
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA4, Common::Input::Key::KEYPAD4 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '4');
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA5, Common::Input::Key::KEYPAD5 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '5');
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA6, Common::Input::Key::KEYPAD6 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '6');
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA7, Common::Input::Key::KEYPAD7 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '7');
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA8, Common::Input::Key::KEYPAD8 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '8');
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA9, Common::Input::Key::KEYPAD9 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '9');
				}
			}
		}

		void SudokuApplication::Draw()
		{
			m_sudokuGridRenderer.RenderSudokuGrid(m_renderTarget, m_sudokuGrid, GetCellSize());

			m_renderTarget.Draw(m_newGameButton);
			m_renderTarget.Draw(m_checkSolutionButton);
			m_renderTarget.Draw(m_resultText);
		}

		void SudokuApplication::Cleanup()
		{
		}

		void SudokuApplication::NewGame()
		{
			if (m_puzzleStrings.size() > 0)
			{
				int randomPuzzleIndex = m_random(0, m_puzzleStrings.size());
				LoadGridFromPuzzleString(m_puzzleStrings[randomPuzzleIndex], m_sudokuGrid);
			}

			m_resultText.SetString("");
		}

		void SudokuApplication::CheckSolution()
		{
			if (m_sudokuGridChecker.IsSolved(m_sudokuGrid))
			{
				m_resultText.SetString("Solved!");
			}
			else
			{
				m_resultText.SetString("Nope");
			}
		}

		float SudokuApplication::GetCellSize() const
		{
			return 100.0f; // temporary
		}

		void SudokuApplication::ReadPuzzleStrings(const std::string& puzzlesFile, std::vector<std::string>& puzzleStrings)
		{
			Common::FileUtils::ReadLinesFromFile(puzzlesFile.c_str(), puzzleStrings);
		}

		void SudokuApplication::LoadGridFromPuzzleString(const std::string & puzzleString, SudokuGrid & outGrid)
		{
			outGrid.ClearGrid();

			for (size_t i = 0; i < 9; ++i)
			{
				for (size_t j = 0; j < 10; ++j)
				{
					if (j < 9)
					{
						size_t index = i * 10 + j;
						char c = puzzleString[index];
						if (c == '.')
						{
							c = ' ';
						}
						outGrid.SetCharacterAt(i, j, c);
						outGrid.SetCellModifiableFlag(i, j, c == ' ');
					}
				}
			}
		}
	}
}
