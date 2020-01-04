//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_STRINGUTILS_HPP
#define VALCANO_STRINGUTILS_HPP

#include <string>
#include <algorithm>
#include <sstream>

namespace mt::util::string {
    std::string to_upper(const std::string &input);

    std::string to_lower(const std::string & input);

    template<typename T>
    std::string to_str(const T &p_obj) {
        std::stringstream ss;
        ss << p_obj;
        return ss.str();
    }
}



#endif //VALCANO_STRINGUTILS_HPP
