//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_SHADERMODULE_HPP
#define VALCANO_SHADERMODULE_HPP

#include "vulkan/vulkan.hpp"

namespace mt::gfx::mtvk{
    class ShaderModule {
        vk::ShaderModule shader_module;

    public:
        ShaderModule(const std::string& module_name);
        ~ShaderModule();
    };
}


#endif //VALCANO_SHADERMODULE_HPP
