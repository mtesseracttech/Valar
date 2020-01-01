//
// Created by MTesseracT on 31/12/2019.
//

#ifndef VALCANO_INSTANCE_HPP
#define VALCANO_INSTANCE_HPP

#include "vulkan/vulkan.hpp"

namespace mt::gfx::mtvk{
	class Instance {
	    vk::Instance instance;
	public:
		explicit Instance(const std::string& app_name);
		~Instance();
	};
}



#endif //VALCANO_INSTANCE_HPP
