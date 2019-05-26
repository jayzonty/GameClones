#include <cstdio>

#include "Common/Library.h"
#include "Common/MemoryLeakTester.h"

#include "TetrisGame.h"

int main() {
	Common::MemoryLeakTester memLeakTester;
	memLeakTester.Start();

	{
		Common::Library::Initialize();

		GameClones::TetrisGame game;
		game.Run();

		Common::Library::Cleanup();
	}

	memLeakTester.End();

	return 0;
}