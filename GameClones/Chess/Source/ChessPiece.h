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
			Type m_type;

		public:
			ChessPiece(Type type)
				: m_isFirstMove(true)
				, m_type(type)
			{
			}

			~ChessPiece() = default;
		};
	}
}