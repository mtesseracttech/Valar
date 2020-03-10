//
// Created by MTesseracT on 28/02/2020.
//

#include "SpvReflection.hpp"

namespace mt::gfx {
	SpvReflection::SpvReflection(const uint32_t * data, uint32_t size) {
		compiler = std::make_shared<spirv_cross::Compiler>(data, size);
		resources = std::make_shared<spirv_cross::ShaderResources>(compiler->get_shader_resources());
	}

	std::shared_ptr<spirv_cross::ShaderResources> SpvReflection::get_shader_reflection_resources() const {
		return resources;
	}

	spirv_cross::SPIRType SpvReflection::get_type_info(const spirv_cross::TypeID id) const {
		return compiler->get_type(id);
	}

	size_t SpvReflection::get_struct_member_size(const spirv_cross::SPIRType id, uint32_t index) const {
		return compiler->get_declared_struct_member_size(id, index);
	}

	std::shared_ptr<spirv_cross::Compiler> SpvReflection::get_compiler() const {
		return compiler;
	}

	std::size_t SpvReflection::get_base_type_size(const spirv_cross::SPIRType::BaseType type) const {
		using BaseType = spirv_cross::SPIRType::BaseType;
		switch(type){
			case BaseType::Void:
				return 0;
			case BaseType::Boolean:
			case BaseType::SByte:
			case BaseType::UByte:
				return sizeof(int8_t);
			case BaseType::Short:
			case BaseType::UShort:
			case BaseType::Half:
				return sizeof(int16_t);
			case BaseType::Int:
			case BaseType::UInt:
			case BaseType::Float:
			case BaseType::AtomicCounter:
				return sizeof(int32_t);
			case BaseType::Int64:
			case BaseType::UInt64:
			case BaseType::Double:
				return sizeof(int64_t);
			default:
				assert(false && "There is no proper solution for this");
		}
	}
}