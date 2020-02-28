//
// Created by MTesseracT on 28/02/2020.
//

#include "SpvReflection.hpp"

namespace mt::gfx {
	SpvReflection::SpvReflection(const std::vector <uint32_t>& spv_source) {
		spirv_cross::Compiler compiler(spv_source);
		resources = std::make_unique<spirv_cross::ShaderResources>(compiler.get_shader_resources());
	}
}