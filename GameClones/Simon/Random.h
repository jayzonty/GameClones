#pragma once

#ifndef ___GameClones_Random_h___
#define ___GameClones_Random_h___

#include <random>
#include <cmath>
#include <chrono>

namespace GameClones {
	class Random {
		std::mt19937 m_randomEngine;
		std::uniform_real_distribution<float> m_distribution;
	public:
		Random(unsigned int seed) : m_randomEngine(seed), m_distribution(0.0f, 1.0f) {}
		Random() : Random(std::chrono::system_clock::now().time_since_epoch().count()) {}

		int operator()(int min, int max) {
			float temp = m_distribution(m_randomEngine);
			return min + ceill((max - min) * temp);
		}

		float operator()(float min, float max) {
			float temp = m_distribution(m_randomEngine);
			return min + (max - min) * temp;
		}
	};
}

#endif // ___GameClones_Random_h___
