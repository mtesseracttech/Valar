//
// Created by mtesseract on 27-5-19.
//

#ifndef VALCANO_LOGGER_HPP
#define VALCANO_LOGGER_HPP


#include <ostream>
#include <string>
#include "util/StringUtils.hpp"

namespace mt {
	enum LogLevel {
		LevelAll = 0,
		LevelDebug = 1,
		LevelInfo = 2,
		LevelError = 3,
		LevelOff = 4
	};

	enum LogType {
		Debug = 1,
		Info = 2,
		Error = 3,
	};

	class Logger {
		LogLevel m_level;

		Logger() : m_level(LevelAll) {};

		~Logger() = default;

		static void print_log(const std::string &p_prefix, const std::string &p_message);

		static void print_error(const std::string &p_prefix, const std::string &p_message);

		static Logger &get_instance();

	public:
		static void log(const std::string &p_message, LogType p_level = Info);

		static void log(std::stringstream &p_os, LogType p_level = Info);

		template<typename T>
		static void log(const T &p_message_object, LogType p_level = Info) {
			log(to_str(p_message_object), p_level);
		}

		static void set_log_level(LogLevel p_level);

		static LogLevel get_log_level();
	};


}


#endif //VALCANO_LOGGER_HPP
