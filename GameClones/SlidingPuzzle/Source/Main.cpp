#include "SlidingPuzzle.h"

#include "Common/MemoryLeakTester.h"
#include "Common/Library.h"

int main() {
	Common::MemoryLeakTester memLeakTester;

	memLeakTester.Start();

	int returnCode = 0;
	{
		if (Common::Library::Initialize()) {
			GameClones::SlidingPuzzle::SlidingPuzzle game;
			game.Run();
		}
		else {
			std::printf("Fail!\n");
		}

		Common::Library::Cleanup();
	}

	memLeakTester.End();

	return returnCode;
}

