#include "SudokuApplication.h"

#include "Common/Window.h"
#include "Common/WindowManager.h"

namespace GameClones
{
	SudokuApplication::SudokuApplication() : BaseApplication("Sudoku")
	{
	}

	SudokuApplication::~SudokuApplication()
	{
	}

	void SudokuApplication::SetupWindow(Common::Window* window)
	{
		window->SetSize(800, 600);
		window->SetTitle("Sudoku");
	}

	void SudokuApplication::Init()
	{
	}

	void SudokuApplication::Update(float deltaTime)
	{
	}

	void SudokuApplication::Draw()
	{
	}

	void SudokuApplication::Cleanup()
	{
	}
}
