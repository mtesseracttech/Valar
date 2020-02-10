//
// Created by mtesseract on 6/23/19.
//

#ifndef VALCANO_TIMEUTILS_HPP
#define VALCANO_TIMEUTILS_HPP

#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace mt{
	class TimeUtils {
		typedef std::chrono::system_clock ChronoClock;

	public:
		static std::string get_time_hh_mm_ss() {
			auto now = ChronoClock::now();
			auto timer = ChronoClock::to_time_t(now);
			std::tm bt = *std::localtime(&timer);
			std::ostringstream oss;
			oss << std::put_time(&bt, "%H:%M:%S");
			return oss.str();
		}
	};
}

#endif //VALCANO_TIMEUTILS_HPP
