//
// Created by mtesseract on 23-6-19.
//

#ifndef VALCANO_TIMER_HPP
#define VALCANO_TIMER_HPP

#include <chrono>
#include <string>

namespace mt {
	class Timer {
	private:
		typedef std::chrono::system_clock ChronoClock;
		typedef std::chrono::duration<double, std::ratio<1> > ChronoSecond;
		std::chrono::time_point<ChronoClock> m_start;

	public:
		Timer() : m_start(ChronoClock::now()) {}

		void reset() {
			m_start = ChronoClock::now();
		}

		double get_elapsed() const {
			return std::chrono::duration_cast<ChronoSecond>(ChronoClock::now() - m_start).count();
		}
	};
}


#endif //VALCANO_TIMER_HPP
