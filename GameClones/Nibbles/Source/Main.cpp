#include "Nibbles.h"

#include "Common/Library.h"
#include "Common/MemoryLeakTester.h"

int main() {
	Common::MemoryLeakTester tester;
	tester.Start();

	{
		Common::Library::Initialize();

		GameClones::Nibbles::Nibbles nibbles;
		nibbles.Run();

		Common::Library::Cleanup();
	}

	tester.End();

	return 0;
}