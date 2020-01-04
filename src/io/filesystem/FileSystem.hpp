//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_FILESYSTEM_HPP
#define VALCANO_FILESYSTEM_HPP

#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

namespace mt::io {
    std::string get_execution_path();

    std::vector<char> read_file(const std::string& file_name);

    bool file_exists(const std::string & file_name);
}

#endif //VALCANO_FILESYSTEM_HPP
