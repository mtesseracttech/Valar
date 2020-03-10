//
// Created by MTesseracT on 28/02/2020.
//

#ifndef VALCANO_SPVREFLECTION_HPP
#define VALCANO_SPVREFLECTION_HPP

#include <spirv_cross.hpp>

namespace mt::gfx {
class SpvReflection {
    std::shared_ptr<spirv_cross::Compiler> compiler;
    std::shared_ptr<spirv_cross::ShaderResources> resources;

public:
    SpvReflection(const uint32_t* data, uint32_t size);

    explicit SpvReflection(const std::vector<uint32_t>& spv_source)
        : SpvReflection(spv_source.data(), spv_source.size())
    {
    }

    std::shared_ptr<spirv_cross::ShaderResources>
    get_shader_reflection_resources() const;

    spirv_cross::SPIRType get_type_info(const spirv_cross::TypeID id) const;

    size_t get_struct_member_size(const spirv_cross::SPIRType id,
        uint32_t index) const;

    std::shared_ptr<spirv_cross::Compiler> get_compiler() const;

    std::size_t get_base_type_size(
        const spirv_cross::SPIRType::BaseType type) const;
};
} // namespace mt::gfx

#endif // VALCANO_SPVREFLECTION_HPP
