#pragma once

#ifndef ___GameClones_Simon_Game_h___
#define ___GameClones_Simon_Game_h___

#include <list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "Timer.h"
#include "MemoryButton.h"
#include "ShaderProgram.h"
#include "Text.h"

namespace GameClones {
	namespace Simon {
		class Game {
			enum class State {
				Start, ShowingMemorizeOrder, WaitingForUserInput, Cooldown, GameOver
			};
			State m_currentState;

			unsigned int m_windowWidth;
			unsigned int m_windowHeight;
			GLFWwindow* m_window;

			Common::Scene* m_rootScene;

			Common::Matrix m_projectionMatrix;

			size_t m_memorizeCount;
			size_t m_maxMemorizeCount;
			std::vector<int> m_memorizeOrder;

			Common::Timer m_blinkTimer;
			Common::Timer m_cooldownTimer;
			size_t m_memorizeButtonIndex;

			std::vector<MemoryButton*> m_memoryButtons;
			Common::ShaderProgram m_textShader;
			Common::Text m_textRender;

			int m_level;
		public:
			Game(unsigned int windowWidth, unsigned int windowHeight, GLFWwindow* window);
			~Game();

			void Init();
			void Update(float deltaTime);
			void Draw();

			void HandleMouseInput(int button, int action, int mods, float mouseX, float mouseY);

		private:
			void GenerateMemorizeOrder(size_t memorizeCount);

			void Reset();
			
		};
	}
}

#endif // ___GameClones_Simon_Game_h___
