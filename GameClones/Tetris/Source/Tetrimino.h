#pragma once

#ifndef ___GameClones_Tetris_Tetrimino_h___
#define ___GameClones_Tetris_Tetrimino_h___

#include "Common/Color.h"

namespace GameClones {
	namespace Tetris {
		struct Block {
			int width;
			int height;
			bool grid[4][4]; // [0][0] is lower-left

			Block() : width(0), height(0) {
				for (size_t i = 0; i < 4; ++i) {
					for (size_t j = 0; j < 4; ++j) {
						grid[i][j] = false;
					}
				}
			}
		};

		struct Tetrimino {
			// 0 = default orientation
			// > 0 = rotated
			Block blocks[4];
			Common::Color color;

			Tetrimino() {
			}
		};
	}
}

#endif // ___GameClones_Tetris_Tetrimino_h___
