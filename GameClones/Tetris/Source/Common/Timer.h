#pragma once

namespace Common {
	class Timer {
		float m_duration;
		float m_timer;

		bool m_isRunning;
		bool m_isFinished;

	public:
		Timer() : m_duration(0.0f), m_timer(0.0f), m_isRunning(false), m_isFinished(false) {
		}

		~Timer() {
		}

		float GetTime() const {
			return m_timer;
		}

		void Update(float deltaTime) {
			if (m_isRunning) {
				m_timer += deltaTime;
				if (m_timer >= m_duration) {
					m_isFinished = true;
					m_isRunning = false;
				}
			}
		}

		void SetDuration(const float& duration) {
			m_duration = duration;
		}

		const float& GetDuration() const {
			return m_duration;
		}

		void Start() {
			m_isRunning = true;
			m_isFinished = false;

			m_timer = 0.0f;
		}

		void Stop() {
			m_isRunning = false;
		}

		bool IsRunning() {
			return m_isRunning;
		}

		bool IsFinished() {
			return m_isFinished;
		}
	};
}