//
// Created by mtesseract on 22-6-19.
//

#ifndef MACH_LOGUTILS_HPP
#define MACH_LOGUTILS_HPP


#if _WINDOWS
// your windows stuff
#else

#include <unistd.h>
#include <limits.h>

#endif

#include <string>
#include <sstream>
#include <aux/exceptions/NotImplemented.hpp>

namespace mt::aux {
	template<typename T>
	static std::string to_str(const T &p_obj) {
		std::stringstream ss;
		ss << p_obj;
		return ss.str();
	}

	static std::string get_execution_path() {
#if _WIN32
		throw NotImplemented("Windows current dir printing is not implemented");
#else
		char buff[PATH_MAX];
		ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
		if (len != -1) {
			buff[len] = '\0';
			return std::string(buff);
		}
#endif
		return std::string();
	}
}

#endif //MACH_LOGUTILS_HPP
