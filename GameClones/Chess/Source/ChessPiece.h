#pragma once

namespace GameClones
{
	namespace Chess
	{
		class ChessPiece
		{
		public:
			enum class Type
			{
				Pawn,
				Rook,
				Knight,
				Bishop,
				Queen,
				King
			};

		private:
			bool m_isFirstMove;
			int m_playerIndex; // In standard 2-player chess, 0 is white, 1 is black
			Type m_type;

		public:
			ChessPiece(Type type, int playerIndex)
				: m_isFirstMove(true)
				, m_playerIndex(playerIndex)
				, m_type(type)
			{
			}

			~ChessPiece() = default;

			bool IsFirstMove()
			{
				return m_isFirstMove;
			}

			void SetIsFirstMove(bool isFirstMove)
			{
				m_isFirstMove = isFirstMove;
			}

			int GetPlayerIndex() const
			{
				return m_playerIndex;
			}

			Type GetType() const
			{
				return m_type;
			}
		};
	}
}