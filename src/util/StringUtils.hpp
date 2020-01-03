//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_STRINGUTILS_HPP
#define VALCANO_STRINGUTILS_HPP

#include <string>
#include <algorithm>
#include <sstream>

namespace mt::util::string {
    static std::string to_upper(const std::string &input) {
        std::string s (input);
        std::transform(s.begin(), s.end(), s.begin(),[](unsigned char c){ return std::toupper(c); } );
        return s;
    }

    static std::string to_lower(const std::string & input){
        std::string s (input);
        std::transform(s.begin(), s.end(), s.begin(),[](unsigned char c){ return std::tolower(c); } );
        return s;
    }

    template<typename T>
    static std::string to_str(const T &p_obj) {
        std::stringstream ss;
        ss << p_obj;
        return ss.str();
    }
}



#endif //VALCANO_STRINGUTILS_HPP
