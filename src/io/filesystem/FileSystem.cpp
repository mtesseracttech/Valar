//
// Created by mtesseract on 04-01-20.
//

#include "FileSystem.hpp"

namespace mt::io {

    std::string get_execution_path() {
        return std::filesystem::current_path().string();
    }

    std::vector<char> read_file(const std::string &file_name) {
        std::ifstream file(file_name, std::ios::ate | std::ios::binary);

        if (!file.is_open()){
            throw std::runtime_error("Failed to open file!: " + file_name);
        }

        auto file_size = static_cast<size_t>(file.tellg());

        std::vector<char> buffer(file_size);

        file.seekg(0);
        file.read(buffer.data(), file_size);
        file.close();

        return buffer;
    }

    bool file_exists(const std::string &file_name) {
        if (FILE *file = fopen(file_name.c_str(), "r")) {
            fclose(file);
            return true;
        } else {
            return false;
        }
    }
}