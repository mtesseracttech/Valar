//
// Created by mtesseract on 03-01-20.
//

#include <io/filesystem/FileSystem.hpp>
#include <map>
#include <aux/logging/Logger.hpp>
#include <aux/exceptions/NotImplemented.hpp>
#include "Shader.hpp"

namespace mt::gfx::mtvk {
    Shader::Shader(const std::string &module_name,
                   const std::shared_ptr<Device> &device, ShaderSourceType module_type) : shader_name(module_name), device(device) {
        switch (module_type){
            case Spirv:
                process_spirv_module(module_name);
                break;
            case GLSL:
                process_glsl_module(module_name);
                break;
        }
    }

    void Shader::process_glsl_module(const std::string &module_name) {
        auto sources = find_sources(glsl_type_ext_pairs, module_name);

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        for(const auto& source : sources){
            auto res = compiler.CompileGlslToSpv(source.source.data(),source.source.size(),get_glsl_shader_kind(source.stage),source.filename.c_str());
            if(res.GetNumErrors() > 0){
                aux::Logger::log("GLSL to SPIR-V Shader Compilation Error:\n" + res.GetErrorMessage(), aux::LogType::Error);
                break;
            } else{
                if(res.GetNumWarnings() > 0){
                    aux::Logger::log(res.GetErrorMessage(), aux::LogType::Error);
                }
                uint32_t size = (res.end() - res.begin()) * sizeof(u_int32_t);
                shader_modules.emplace_back(source.stage, create_shader_module(res.begin(), size));
            }
        }
    }

    void Shader::process_spirv_module(const std::string &module_name) {
        auto sources = find_sources(spirv_type_ext_pairs, module_name);
        throw aux::NotImplemented("This is yet to be implemented, but should be the easier");
    }

    Shader::~Shader() {
        for(const auto& shader_module : shader_modules){
            if(shader_module.second){
                device->get_device().destroyShaderModule(shader_module.second);
            }
        }
        shader_modules.clear();
    }

    std::vector<Shader::ShaderSourceData> Shader::find_sources(const std::vector<TypeExtPair> &type_ext_pairs,
                                                               const std::string &module_name) {
        std::stringstream base_path;
        base_path << "shaders/" << module_name << "/" << module_name;
        auto file_name_no_ext = base_path.str();

        std::vector<Shader::ShaderSourceData> found_sources;

        for (const auto &ext_type_pair : type_ext_pairs) {
            std::string full_file_path = file_name_no_ext + ext_type_pair.second;
            if (io::file_exists(full_file_path)) {
                Shader::ShaderSourceData source_data;
                source_data.filename = full_file_path;
                source_data.source = io::read_file(full_file_path);
                source_data.stage = ext_type_pair.first;
                found_sources.push_back(source_data);
            }
        }

        if(found_sources.empty()){
            throw std::runtime_error("Found no source files for '" + module_name + "'");
        }

        std::stringstream found_message;
        found_message << "Found " << found_sources.size() << " SPIR-V Source Files for the Shader Module " << module_name;

        return found_sources;
    }

    shaderc_shader_kind Shader::get_glsl_shader_kind(vk::ShaderStageFlagBits flags) {
        switch(flags){
            case vk::ShaderStageFlagBits::eVertex:
                return shaderc_glsl_vertex_shader;
            case vk::ShaderStageFlagBits::eTessellationControl:
                return shaderc_glsl_tess_control_shader;
            case vk::ShaderStageFlagBits::eTessellationEvaluation:
                return shaderc_glsl_tess_evaluation_shader;
            case vk::ShaderStageFlagBits::eGeometry:
                return shaderc_glsl_geometry_shader;
            case vk::ShaderStageFlagBits::eFragment:
                return shaderc_glsl_fragment_shader;
            case vk::ShaderStageFlagBits::eCompute:
                return shaderc_glsl_compute_shader;
            default:
                throw std::runtime_error("No corresponding glsl shader type");
        }
    }

    vk::ShaderModule Shader::create_shader_module(const uint32_t *data, uint32_t size) {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo.codeSize = size;
        createInfo.pCode = data;
        return device->get_device().createShaderModule(createInfo);
    }

    std::vector<vk::PipelineShaderStageCreateInfo> Shader::generate_shader_stage_create_infos() {
        std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;
        for(const auto& module : shader_modules){
            vk::PipelineShaderStageCreateInfo create_info;
            create_info.stage = module.first;
            create_info.module = module.second;
            create_info.pName = "main";
        }
        return shader_stages;
    }
}