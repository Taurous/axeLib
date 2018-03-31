#pragma once

#include <chrono>

namespace axe
{
	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::milliseconds m_secs;
	typedef std::chrono::time_point<Clock> TimePoint;

	class Timer
	{
	private:
		Clock::time_point epoch;

	public:
		void start()
		{
			epoch = Clock::now();
		}

		m_secs restart()
		{
			m_secs elapsed = std::chrono::duration_cast<m_secs>(Clock::now() - epoch);
			epoch = Clock::now();
			return elapsed;
		}

		m_secs elapsed()
		{
			return std::chrono::duration_cast<m_secs>(Clock::now() - epoch);
		}

		TimePoint now()
		{
			return Clock::now();
		}
	};
}