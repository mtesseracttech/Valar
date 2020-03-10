//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_STRINGUTILS_HPP
#define VALCANO_STRINGUTILS_HPP

#include <algorithm>
#include <sstream>
#include <string>

namespace mt {
std::string to_upper(const std::string& input);

std::string to_lower(const std::string& input);

template <typename T>
std::string to_str(const T& p_obj)
{
    std::stringstream ss;
    ss << p_obj;
    return ss.str();
}
} // namespace mt

#endif // VALCANO_STRINGUTILS_HPP
