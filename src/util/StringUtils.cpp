//
// Created by mtesseract on 04-01-20.
//

#include "StringUtils.hpp"

namespace mt {
std::string to_upper(const std::string& input)
{
    std::string s(input);
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return s;
}

std::string to_lower(const std::string& input)
{
    std::string s(input);
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return s;
}
} // namespace mt