//
// Created by mtesseract on 23-6-19.
//

#ifndef MACH_TIMER_HPP
#define MACH_TIMER_HPP

#include <chrono>
#include <string>

namespace mt::aux {
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


#endif //MACH_TIMER_HPP
