#include <cstdio>

#include "Common/Library.h"
#include "Common/MemoryLeakTester.h"

#include "SudokuApplication.h"

int main() {
	Common::MemoryLeakTester memLeakTester;
	memLeakTester.Start();

	{
		Common::Library::Initialize();

		GameClones::Sudoku::SudokuApplication game;
		game.Run();

		Common::Library::Cleanup();
	}

	memLeakTester.End();

	return 0;
}