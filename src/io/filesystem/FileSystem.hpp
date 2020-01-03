//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_FILESYSTEM_HPP
#define VALCANO_FILESYSTEM_HPP

#include <filesystem>
#include <string>

namespace mt::io {
    static std::string get_execution_path() {
        return std::filesystem::current_path().string();
    }
}

#endif //VALCANO_FILESYSTEM_HPP
