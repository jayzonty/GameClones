#include "SudokuApplication.h"

#include <iostream>

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
			, m_renderTarget()
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
			window->SetSize(900, 900);
			window->SetTitle("Sudoku");

			m_renderTarget.SetSize(900, 900);
		}

		void SudokuApplication::Init()
		{
			ReadPuzzleStrings("Resources/Data/puzzles.txt", m_puzzleStrings);

			if (m_puzzleStrings.size() > 0)
			{
				LoadGridFromPuzzleString(m_puzzleStrings[0], m_sudokuGrid);
			}
		}

		void SudokuApplication::Update(float deltaTime)
		{
			size_t numRows = 0, numColumns = 0;
			m_sudokuGrid.GetSize(numRows, numColumns);

			if (Common::Input::IsPressed(Common::Input::LEFT_MOUSE))
			{
				int mouseX = 0, mouseY = 0;
				Common::Input::GetMousePosition(&mouseX, &mouseY);

				m_selectedRow = static_cast<size_t>(mouseY / GetCellSize());
				m_selectedColumn = static_cast<size_t>(mouseX / GetCellSize());
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
				bool pressed = false;
				// TODO: Should have a better way of doing this
				if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA1, Common::Input::Key::KEYPAD1 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '1');
					pressed = true;
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA2, Common::Input::Key::KEYPAD2 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '2');
					pressed = true;
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA3, Common::Input::Key::KEYPAD3 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '3');
					pressed = true;
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA4, Common::Input::Key::KEYPAD4 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '4');
					pressed = true;
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA5, Common::Input::Key::KEYPAD5 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '5');
					pressed = true;
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA6, Common::Input::Key::KEYPAD6 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '6');
					pressed = true;
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA7, Common::Input::Key::KEYPAD7 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '7');
					pressed = true;
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA8, Common::Input::Key::KEYPAD8 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '8');
					pressed = true;
				}
				else if (Common::Input::IsAnyPressed({ Common::Input::Key::ALPHA9, Common::Input::Key::KEYPAD9 }))
				{
					m_sudokuGrid.SetCharacterAt(m_selectedRow, m_selectedColumn, '9');
					pressed = true;
				}

				if (pressed)
				{
					if (m_sudokuGridChecker.IsSolved(m_sudokuGrid))
					{
						std::cout << "You win!" << std::endl;
					}
				}
			}
		}

		void SudokuApplication::Draw()
		{
			m_sudokuGridRenderer.RenderSudokuGrid(m_renderTarget, m_sudokuGrid, GetCellSize());
		}

		void SudokuApplication::Cleanup()
		{
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
