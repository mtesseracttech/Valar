//
// Created by MTesseracT on 28/02/2020.
//

#ifndef VALCANO_SPVREFLECTION_HPP
#define VALCANO_SPVREFLECTION_HPP

#include <spirv_cross.hpp>

namespace mt::gfx{
	class SpvReflection {
		std::unique_ptr<spirv_cross::ShaderResources> resources = nullptr;
	public:
		explicit SpvReflection(const std::vector<uint32_t>& spv_source);
	};
}



#endif //VALCANO_SPVREFLECTION_HPP
