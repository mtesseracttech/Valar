//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_STRINGUTILS_HPP
#define VALCANO_STRINGUTILS_HPP

#include <string>

namespace mt::util{
	class StringUtils {
	public:
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

	};
}



#endif //VALCANO_STRINGUTILS_HPP
