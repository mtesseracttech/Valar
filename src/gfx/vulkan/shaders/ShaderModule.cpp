//
// Created by mtesseract on 03-01-20.
//

#include <io/filesystem/FileSystem.hpp>
#include <map>
#include <aux/logging/Logger.hpp>
#include "ShaderModule.hpp"

namespace mt::gfx::mtvk {
    ShaderModule::ShaderModule(const std::string &module_name,
            const std::shared_ptr<Device> &device, ShaderModuleType module_type) : shader_name(module_name), device(device) {
        switch (module_type){
            case Spirv:
                process_spirv_module(module_name, device);
                break;
            case GLSL:
                process_glsl_module(module_name, device);
                break;
        }
    }

    void ShaderModule::process_glsl_module(const std::string &module_name, const std::shared_ptr<Device> &device) {
        auto sources = find_sources(glsl_type_ext_pairs, module_name);

        shaderc::Compiler compiler;

        for(const auto& source : sources){
            auto it = std::find(vk_shaderc_type_pairs.begin(), vk_shaderc_type_pairs.end(), source.stage);
            if(it != vk_shaderc_type_pairs.end()){
                auto res = compiler.CompileGlslToSpv(source.source.data(), source.source.size(), it->second, source.filename.c_str());
                if(res.GetNumErrors() > 0){
                    aux::Logger::log(res.GetErrorMessage(), aux::LogType::Error);
                    throw std::runtime_error("An error happened while compiling the GLSL shaders to SPIR-V");
                } else{
                    if(res.GetNumWarnings()){
                        aux::Logger::log(res.GetErrorMessage(), aux::LogType::Error);
                    }
                }
            }
        }
    }

    void ShaderModule::process_spirv_module(const std::string &module_name, const std::shared_ptr<Device> &device) {
        auto sources = find_sources(spirv_type_ext_pairs, module_name);
    }

    ShaderModule::~ShaderModule() {
        for(const auto& shader_module : shader_modules){
            if(shader_module.second){
                device->get_device().destroyShaderModule(shader_module.second);
            }
        }
    }

    std::vector<ShaderModule::ShaderSourceData> ShaderModule::find_sources(const std::vector<TypeExtPair> &type_ext_pairs,
                               const std::string &module_name) {
        std::stringstream base_path;
        base_path << "shaders/" << module_name << "/" << module_name;
        auto file_name_no_ext = base_path.str();

        std::vector<ShaderModule::ShaderSourceData> found_sources;

        for (const auto &ext_type_pair : type_ext_pairs) {
            std::string full_file_path = file_name_no_ext + ext_type_pair.second;
            if (io::file_exists(full_file_path)) {
                ShaderModule::ShaderSourceData source_data;
                source_data.filename = module_name + ext_type_pair.second;
                source_data.source = io::read_file(full_file_path);
                source_data.stage = ext_type_pair.first;
                found_sources.push_back(source_data);
                //found_sources.emplace_back();
            }
        }

        if(found_sources.empty()){
            throw std::runtime_error("Found no source files for '" + module_name + "'");
        }

        std::stringstream found_message;
        found_message << "Found " << found_sources.size() << " SPIR-V Source Files for the Shader Module " << module_name;

        for(const auto& source : found_sources){
            found_message << '\n' << source.filename << '\n' << source.source.data();
        }
        aux::Logger::log(found_message);

        return found_sources;
    }


}