//
// Created by mtesseract on 27-5-19.
//

#include "Logger.hpp"

#include <aux/time/TimeUtils.hpp>
#include <iostream>
#include <sstream>

namespace mt {
Logger& Logger::get_instance()
{
    static Logger instance;
    return instance;
}

void Logger::print_log(const std::string& p_prefix,
    const std::string& p_message)
{
    std::stringstream output;
    output << "[" << TimeUtils::get_time_hh_mm_ss() << "][" << p_prefix << "] "
           << p_message;
    std::cout << output.str() << std::endl;
}

void Logger::print_error(const std::string& p_prefix,
    const std::string& p_message)
{
    // This makes sure that everything printed from cout is done before the cerr
    // is done
    std::cout << std::flush;
    std::stringstream output;
    output << "[" << TimeUtils::get_time_hh_mm_ss() << "][" << p_prefix << "] "
           << p_message;
    std::cerr << output.str() << std::endl;
}

void Logger::log(const std::string& p_message, LogType p_level)
{
    Logger logger = get_instance();

    if (static_cast<int>(p_level) < static_cast<int>(logger.m_level))
        return;

    switch (p_level) {
    case Debug:
        logger.print_log("Debug", p_message);
        break;
    case Info:
        logger.print_log("Info", p_message);
        break;
    case Error:
        logger.print_error("Error", p_message);
        break;
    }
}

void Logger::log(std::stringstream& p_os, LogType p_level)
{
    log(p_os.str(), p_level);
}

void Logger::set_log_level(LogLevel p_level)
{
    get_instance().m_level = p_level;
}

LogLevel Logger::get_log_level() { return get_instance().m_level; }
} // namespace mt
