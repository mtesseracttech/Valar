//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_SHADERMODULE_HPP
#define VALCANO_SHADERMODULE_HPP

#include <gfx/vulkan/display/Device.hpp>
#include <shaderc/shaderc.hpp>
#include "vulkan/vulkan.hpp"
#include <map>


namespace mt::gfx::mtvk{
    enum ShaderModuleType{
        Spirv,
        GLSL
    };

    enum SourceFileType{
        Vertex,
        Geometry
    };

    class ShaderModule {
        struct ShaderSourceData{
            vk::ShaderStageFlagBits stage;
            std::string filename;
            std::vector<char> source;
        };

        typedef std::pair<vk::ShaderStageFlagBits, std::string> TypeExtPair;

        std::vector<std::pair<vk::ShaderStageFlagBits,vk::ShaderModule>> shader_modules;
        std::string shader_name;
        std::shared_ptr<Device> device;

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

        std::map<vk::ShaderStageFlagBits, shaderc_shader_kind> vk_shaderc_type_pairs{
                {vk::ShaderStageFlagBits::eVertex, shaderc_glsl_vertex_shader},
                {vk::ShaderStageFlagBits::eTessellationControl, shaderc_glsl_tess_control_shader},
                {vk::ShaderStageFlagBits::eTessellationEvaluation, shaderc_glsl_tess_evaluation_shader},
                {vk::ShaderStageFlagBits::eGeometry, shaderc_glsl_geometry_shader},
                {vk::ShaderStageFlagBits::eFragment, shaderc_glsl_fragment_shader},
                {vk::ShaderStageFlagBits::eCompute, shaderc_glsl_compute_shader}
        };

        std::vector<ShaderSourceData>
        find_sources(const std::vector<TypeExtPair> &type_ext_pairs,
                     const std::string &module_name);

        void process_glsl_module(const std::string &module_name, const std::shared_ptr<Device> &device);

        void process_spirv_module(const std::string &module_name, const std::shared_ptr<Device> &device);

    public:
        ShaderModule(const std::string& module_name, const std::shared_ptr<Device>& device, ShaderModuleType module_type);
        ~ShaderModule();


    };
}


#endif //VALCANO_SHADERMODULE_HPP
