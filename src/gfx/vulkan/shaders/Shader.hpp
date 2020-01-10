//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_SHADER_HPP
#define VALCANO_SHADER_HPP

#include <gfx/vulkan/display/Device.hpp>
#include <shaderc/shaderc.hpp>
#include "vulkan/vulkan.hpp"
#include <map>


namespace mt::gfx::mtvk{
    enum ShaderSourceType{
        Spirv,
        GLSL
    };


    class Shader {
        typedef std::pair<vk::ShaderStageFlagBits, std::string> TypeExtPair;

        struct ShaderSourceData{
            vk::ShaderStageFlagBits stage;
            std::string filename;
            std::vector<char> source;
        };

        std::vector<TypeExtPair> glsl_type_ext_pairs {
                {vk::ShaderStageFlagBits::eVertex, ".vert"},
                {vk::ShaderStageFlagBits::eTessellationControl, ".tesc"},
                {vk::ShaderStageFlagBits::eTessellationEvaluation, ".tese"},
                {vk::ShaderStageFlagBits::eGeometry, ".geom"},
                {vk::ShaderStageFlagBits::eFragment, ".frag"},
                {vk::ShaderStageFlagBits::eCompute, ".comp"}
        };

        std::vector<TypeExtPair> spirv_type_ext_pairs {
                {vk::ShaderStageFlagBits::eVertex, ".vert.spv"},
                {vk::ShaderStageFlagBits::eTessellationControl, ".tesc.spv"},
                {vk::ShaderStageFlagBits::eTessellationEvaluation, ".tese.spv"},
                {vk::ShaderStageFlagBits::eGeometry, ".geom.spv"},
                {vk::ShaderStageFlagBits::eFragment, ".frag.spv"},
                {vk::ShaderStageFlagBits::eCompute, ".comp.spv"}
        };

        std::string shader_name;
        std::vector<std::pair<vk::ShaderStageFlagBits,vk::ShaderModule>> shader_modules;
        std::shared_ptr<Device> device;

        std::vector<ShaderSourceData>
        find_sources(const std::vector<TypeExtPair> &type_ext_pairs,
                     const std::string &module_name);

        void process_glsl_module(const std::string &module_name);

        void process_spirv_module(const std::string &module_name);

        shaderc_shader_kind get_glsl_shader_kind(vk::ShaderStageFlagBits flags);

        vk::ShaderModule create_shader_module(const uint32_t* data, uint32_t size);
    public:
        Shader(const std::string& module_name, const std::shared_ptr<Device>& device, ShaderSourceType module_type);
        ~Shader();

        std::vector<vk::PipelineShaderStageCreateInfo> create_shader_stage_create_infos() const;
    };
}


#endif //VALCANO_SHADER_HPP
