//
// Created by mtesseract on 03-01-20.
//

#include <io/filesystem/FileSystem.hpp>
#include <map>
#include <aux/logging/Logger.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>
#include <glslang/Public/ShaderLang.h>
#include "ShaderModule.hpp"

namespace mt::gfx::mtvk {
    ShaderModule::ShaderModule(const std::string &module_name,
            const std::shared_ptr<Device> &device) : shader_name(module_name), device(device) {
        std::stringstream base_path;
        base_path << "shaders/" << module_name << "/" << module_name;
        auto file_name_no_ext = base_path.str();

        std::vector<std::pair<vk::ShaderStageFlagBits, std::vector<char>>> found_sources;

        for (const auto &ext_type_pair : spirv_type_ext_pairs) {
            std::string full_file_path = file_name_no_ext + ext_type_pair.second;
            if (io::file_exists(full_file_path)) {
                found_sources.push_back(std::make_pair<>(ext_type_pair.first, io::read_file(full_file_path)));
            }
        }

        if(found_sources.empty()){
            throw std::runtime_error("Found no source files for '" + module_name + "'");
        }

        std::stringstream found_message;
        found_message << "Found " << found_sources.size() << " SPIR-V Source Files for the Shader Module " << module_name;
        aux::Logger::log(found_message);
    }

    ShaderModule::~ShaderModule() {
        for(const auto& shader_module : shader_modules){
            if(shader_module.second){
                device->get_device().destroyShaderModule(shader_module.second);
            }
        }
    }
}