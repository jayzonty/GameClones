#ifndef ___Common_ListUtils_h___
#define ___Common_ListUtils_h___

#include "Random.h"

#include <vector>
#include <list>
#include <iterator>

namespace Common
{
	namespace ListUtils
	{
		template <class T>
		void Shuffle(std::vector<T>& list, unsigned int seed)
		{
			Random random(seed);
			for (int i = 0; i < list.size(); ++i)
			{
				int swapIndex = random(i, list.size() - 1);
				T temp = list[i];
				list[i] = list[swapIndex];
				list[swapIndex] = temp;
			}
		}

		template <class T>
		void Shuffle(std::vector<T>& list)
		{
			unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
			Shuffle(list, seed);
		}
	}
}

#endif // ___Common_ListUtils_h___
