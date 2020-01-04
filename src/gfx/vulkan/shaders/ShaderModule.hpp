//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_SHADERMODULE_HPP
#define VALCANO_SHADERMODULE_HPP

#include <gfx/vulkan/display/Device.hpp>
#include "vulkan/vulkan.hpp"

namespace mt::gfx::mtvk{
    class ShaderModule {
        std::vector<std::pair<vk::ShaderStageFlagBits,vk::ShaderModule>> shader_modules;
        std::string shader_name;
        std::shared_ptr<Device> device;


        std::vector<std::pair<vk::ShaderStageFlagBits, std::string>> spirv_type_ext_pairs {
                {vk::ShaderStageFlagBits::eVertex, ".vert.spv"},
                {vk::ShaderStageFlagBits::eTessellationControl, ".tesc.spv"},
                {vk::ShaderStageFlagBits::eTessellationEvaluation, ".tese.spv"},
                {vk::ShaderStageFlagBits::eGeometry, ".geom.spv"},
                {vk::ShaderStageFlagBits::eFragment, ".frag.spv"},
                {vk::ShaderStageFlagBits::eCompute, ".comp.spv"}
        };

    public:
        ShaderModule(const std::string& module_name, const std::shared_ptr<Device>& device);
        ~ShaderModule();
    };
}


#endif //VALCANO_SHADERMODULE_HPP
