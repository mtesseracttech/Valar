//
// Created by mtesseract on 22-6-19.
//

#ifndef MACH_LOGUTILS_HPP
#define MACH_LOGUTILS_HPP


#include <string>
#include <sstream>
#include <filesystem>

namespace mt::aux {
	template<typename T>
	static std::string to_str(const T &p_obj) {
		std::stringstream ss;
		ss << p_obj;
		return ss.str();
	}

	static std::string get_execution_path() {
	    return std::filesystem::current_path().string();
	}
}

#endif //MACH_LOGUTILS_HPP
