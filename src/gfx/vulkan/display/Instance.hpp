//
// Created by MTesseracT on 31/12/2019.
//

#ifndef VALCANO_INSTANCE_HPP
#define VALCANO_INSTANCE_HPP

#include "vulkan/vulkan.hpp"

namespace mt::gfx::mtvk{
	class Instance {

#ifdef NDEBUG
		const bool enable_validation_layers = false;
#else
		const bool enable_validation_layers = true;
#endif

		const std::vector<const char*> validation_layers = {
				"VK_LAYER_KHRONOS_validation"
		};

	    vk::Instance instance;
	    bool validation_layers_enabled = false;

		bool check_validation_layer_support();
		std::vector<const char*> get_required_extensions();

	public:
		explicit Instance(const std::string& app_name);
		bool has_validation_layers() const;
        std::vector<const char*> get_validation_layers() const;

		vk::Instance get_instance();

		~Instance();
	};
}



#endif //VALCANO_INSTANCE_HPP
