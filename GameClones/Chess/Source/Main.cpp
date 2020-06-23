#include <cstdio>

#include "Common/Library.h"
#include "Common/MemoryLeakTester.h"

#include "ChessApplication.h"

int main() {
	Common::MemoryLeakTester memLeakTester;
	memLeakTester.Start();

	{
		Common::Library::Initialize();

		GameClones::Chess::ChessApplication game;
		game.Run();

		Common::Library::Cleanup();
	}

	memLeakTester.End();

	return 0;
}