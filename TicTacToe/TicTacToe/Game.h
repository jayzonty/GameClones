#pragma once

#ifndef ___GameClones_TicTacToe_Game_h___
#define ___GameClones_TicTacToe_Game_h___

namespace GameClones {
	namespace TicTacToe {
		class Game {
			int m_board[9];

			int m_turn;
			int m_numTurns;

		public:
			Game() : m_turn(0), m_numTurns(0) {
				ClearBoard();
			}

			int GetTurn() const {
				return m_turn;
			}

			void ResetGame() {
				ClearBoard();

				m_turn = 0;
				m_numTurns = 0;
			}

			bool PlaceAt(int player, int row, int column) {
				int index = GetFlattenedIndex(row, column);
				if (m_board[index] != -1) {
					return false;
				}

				m_board[index] = player;
				m_turn = (m_turn + 1) % 2;
				++m_numTurns;

				return true;
			}

			// Returns -1 if there are no winners yet
			// Otherwise, returns the player index of the winner (0 or 1)
			int GetWinner() const {
				if (DidPlayerWin(0)) {
					return 0;
				}

				if (DidPlayerWin(1)) {
					return 1;
				}

				return -1;
			}

			bool IsDrawGame() const {
				return (m_numTurns == 9);
			}

			// Returns -1 if the cell is empty.
			// Otherwise, returns the index of the player that owns the cell
			int Get(int row, int column) const {
				return m_board[GetFlattenedIndex(row, column)];
			}

		private:
			int GetFlattenedIndex(int row, int column) const {
				return row * 3 + column;
			}

			void ClearBoard() {
				for (int i = 0; i < 9; i++) {
					m_board[i] = -1;
				}
			}

			bool DidPlayerWin(int player) const {
				// Check rows
				for (int row = 0; row < 3; ++row) {
					if ((m_board[GetFlattenedIndex(row, 0)] == player) &&
						(m_board[GetFlattenedIndex(row, 1)] == player) &&
						(m_board[GetFlattenedIndex(row, 2)] == player)) {
						return true;
					}
				}

				// Check columns
				for (int column = 0; column < 3; ++column) {
					if ((m_board[GetFlattenedIndex(0, column)] == player) &&
						(m_board[GetFlattenedIndex(1, column)] == player) &&
						(m_board[GetFlattenedIndex(2, column)] == player)) {
						return true;
					}
				}

				// Check diagonals
				if ((m_board[GetFlattenedIndex(0, 0)] == player) &&
					(m_board[GetFlattenedIndex(1, 1)] == player) &&
					(m_board[GetFlattenedIndex(2, 2)] == player)) {
					return true;
				}
				if ((m_board[GetFlattenedIndex(2, 0)] == player) &&
					(m_board[GetFlattenedIndex(1, 1)] == player) &&
					(m_board[GetFlattenedIndex(0, 2)] == player)) {
					return true;
				}

				return false;
			}
		};
	}
}

#endif // ___GameClones_TicTacToe_Game_h___
