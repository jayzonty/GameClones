#pragma once

#ifndef ___GameClones_Minesweeper_Game_h___
#define ___GameClones_Minesweeper_Game_h___

#include <vector>
#include <cstdint>

#include "Random.h"

namespace GameClones {
	namespace MineSweeper {
		class Game {
			// Lower 4-bits represent the actual value of the cell (0xFF for bomb, number of adjacent bombs otherwise)
			// Upper 4-bits represent state of the cell (0 - unrevealed, 1 - revealed, 2 - flagged)
			std::vector<int8_t> m_board;

			int m_numRows;
			int m_numColumns;
			int m_numBombs;

			Random m_random;
			int8_t m_gameState; // -1 = lose, 1 = win, 0 = not finished

		public:
			Game() : m_numRows(0), m_numColumns(0), m_numBombs(0), m_gameState(0) {}

			void SetSize(int numRows, int numColumns) {
				m_numRows = numRows;
				m_numColumns = numColumns;

				m_board.resize(numRows * numColumns, 0);
				ClearBoard();
			}

			int GetNumRows() const {
				return m_numRows;
			}

			int GetNumColumns() const {
				return m_numColumns;
			}

			int GetNumBombs() const {
				return m_numBombs;
			}

			bool DidPlayerWin() const {
				return m_gameState == 1;
			}

			bool DidPlayerLose() const {
				return m_gameState == -1;
			}

			// 0x0F(15) means a bomb
			// 0 means blank space
			// Otherwise, returns the number of bombs adjacent to the cell
			int8_t GetCellValue(int row, int column) const {
				return GetRawCell(row, column) & 0x0F;
			}

			bool IsCellRevealed(int row, int column) const {
				return (GetRawCell(row, column) & 0x10) > 0;
			}

			bool IsCellFlagged(int row, int column) const {
				return (GetRawCell(row, column) & 0x20) > 0;
			}

			bool IsBomb(int row, int column) const {
				return GetCellValue(row, column) == 0x0F;
			}

			void RevealCell(int row, int column) {
				if (IsBomb(row, column)) {
					RevealAllBombs();

					m_gameState = -1; // Player lost
				}
				else {
					FloodFillReveal(row, column);

					int cellsToReveal = m_board.size() - m_numBombs;
					if (GetNumCellsRevealed() == cellsToReveal) {
						m_gameState = 1;
					}
				}
			}

			void ToggleCellFlag(int row, int column) {
				ToggleCellProperty(row, column, 0x20);
			}

			void GenerateBoard() {
				GenerateBoard(m_numBombs);
			}

			void ClearBoard() {
				for (int i = 0; i < m_board.size(); ++i) {
					m_board[i] = 0;
				}
			}

			void GenerateBoard(int numBombs) {
				m_numBombs = numBombs;

				ClearBoard();

				m_gameState = 0;


				// Set the first numBombs elements of the array to "bombs"
				// Then shuffle the list to determine the position of the bombs
				// Finally, compute the number of adjacent bombs for each cell
				if (numBombs > m_numRows * m_numColumns) {
					numBombs = m_numRows * m_numColumns;
				}

				for (int i = 0; i < numBombs; ++i) {
					m_board[i] = 0x0F;
				}

				for (int i = 0; i < m_board.size(); ++i) {
					int swapIndex = m_random(i, m_board.size() - 1);
					uint8_t temp = m_board[i];
					m_board[i] = m_board[swapIndex];
					m_board[swapIndex] = temp;
				}

				for (int row = 0; row < m_numRows; ++row) {
					for (int column = 0; column < m_numColumns; ++column) {
						int index = GetFlattenedIndex(row, column);
						if (m_board[index] != 0x0F) {
							if (GetCellValue(row - 1, column - 1) == 0x0F) ++m_board[index];
							if (GetCellValue(row - 1, column) == 0x0F) ++m_board[index];
							if (GetCellValue(row - 1, column + 1) == 0x0F) ++m_board[index];
							if (GetCellValue(row, column - 1) == 0x0F) ++m_board[index];
							if (GetCellValue(row, column + 1) == 0x0F) ++m_board[index];
							if (GetCellValue(row + 1, column - 1) == 0x0F) ++m_board[index];
							if (GetCellValue(row + 1, column) == 0x0F) ++m_board[index];
							if (GetCellValue(row + 1, column + 1) == 0x0F) ++m_board[index];
						}
					}
				}
			}

		private:
			void FloodFillReveal(int row, int column) {
				if (IsCellValid(row, column) && !IsCellRevealed(row, column)) {
					SetCellProperty(row, column, 0x10);

					if (GetCellValue(row, column) == 0) {
						FloodFillReveal(row - 1, column);
						FloodFillReveal(row, column - 1);
						FloodFillReveal(row, column + 1);
						FloodFillReveal(row + 1, column);
					}
				}
			}

			void SetCellProperty(int row, int column, int8_t property) {
				if (IsCellValid(row, column)) {
					int index = GetFlattenedIndex(row, column);
					m_board[index] = (m_board[index] & 0x0F) | property;
				}
			}

			void ToggleCellProperty(int row, int column, int8_t property) {
				if (IsCellValid(row, column)) {
					int index = GetFlattenedIndex(row, column);
					m_board[index] = m_board[index] ^ property;
				}
			}

			int8_t GetRawCell(int row, int column) const {
				if ((row < 0) || (row >= m_numRows) || (column < 0) || (column >= m_numColumns)) {
					return 0;
				}

				int index = GetFlattenedIndex(row, column);
				return m_board[index];
			}

			int GetFlattenedIndex(int row, int column) const {
				return row * m_numColumns + column;
			}

			bool IsCellValid(int row, int column) {
				return (0 <= row && row < m_numRows) && (0 <= column && column < m_numColumns);
			}

			void RevealAllBombs() {
				for (int row = 0; row < m_numRows; ++row) {
					for (int column = 0; column < m_numColumns; ++column) {
						if (IsBomb(row, column)) {
							SetCellProperty(row, column, 0x10);
						}
					}
				}
			}

			int GetNumCellsRevealed() const {
				int ret = 0;
				for (int i = 0; i < m_board.size(); ++i) {
					if ((m_board[i] & 0x10) > 0) {
						++ret;
					}
				}

				return ret;
			}
		};
	}
}

#endif // ___GameClones_Minesweeper_Game_h___
