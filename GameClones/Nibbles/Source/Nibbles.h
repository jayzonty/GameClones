#pragma once

#ifndef ___GameClones_Nibbles_Nibbles_h___
#define ___GameClones_Nibbles_Nibbles_h___

#include "Common/BaseApplication.h"
#include "Common/RectangleShape.h"
#include "Common/Rect.h"
#include "Common/RenderTarget.h"
#include "Common/Timer.h"
#include "Common/Font.h"
#include "Common/Text.h"

#include <vector>
#include <list>

namespace GameClones {
	namespace Nibbles {
		class Nibbles : public Common::BaseApplication {
		private:
			struct IntVec2 {
				int x, y;
			};

			enum class State {
				Start, Play, Lose, Win
			};

		private:
			const unsigned int WINDOW_WIDTH = 800;
			const unsigned int WINDOW_HEIGHT = 600;
			State m_currentState;

			unsigned int m_numRows, m_numColumns;
			float m_cellSize;
			
			std::vector<unsigned char> m_grid;

			Common::Rect<float> m_gameBounds;

			Common::RectangleShape m_rectangleShape;
			Common::RectangleShape m_gameBoundsBorderShape;
			Common::RectangleShape m_gameBoundsShape;

			Common::Font m_textFont;
			Common::Text m_statusText;

			Common::RenderTarget m_renderTarget;

			std::list<IntVec2> m_snakeBody;

			std::list<IntVec2> m_inputBuffer;
			unsigned int m_inputBufferLimit;

			int m_foodX, m_foodY;

			Common::Color m_snakeBodyColor;
			Common::Color m_snakeHeadColor;
			Common::Color m_foodColor;

			int m_snakeDirX, m_snakeDirY;
			Common::Timer m_movementTickTimer;
			int m_snakeSpeed;

			int m_score;

		public:
			Nibbles();
			~Nibbles();

			void Init() override;

			void Update(float deltaTime) override;

			void Draw() override;

			void Cleanup() override;

		private:
			void InitializeGrid();
			void ClearGrid();

			void ProcessInputBuffer();
			int UpdateGame(float deltaTime);

			void Reset();
			void ResetSnake();
			int UpdateSnakePosition(float deltaTime);
			void ExtendSnake(unsigned int amount);

			bool SpawnFood();

			void SetCellType(int x, int y, unsigned char type);
			unsigned char GetCellType(int x, int y);

			void WrapCell(int* cellX, int* cellY);

			void GetDirectionFromIndex(int index, int* directionX, int* directionY);
			void GetRandomDirection(int* directionX, int* directionY);
		};
	}
}

#endif // ___GameClones_Nibbles_Nibbles_h___
