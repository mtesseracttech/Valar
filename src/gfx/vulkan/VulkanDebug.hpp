//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_VULKANDEBUG_HPP
#define VALCANO_VULKANDEBUG_HPP

#include "Instance.hpp"

namespace mt::gfx::mtvk {
	class VulkanDebug {

	protected:
		std::shared_ptr<Instance> instance;
		vk::DebugUtilsMessengerEXT debug_messenger;


		static VKAPI_ATTR vk::Bool32 VKAPI_CALL vulkan_debug_callback(
				VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
				VkDebugUtilsMessageTypeFlagsEXT message_type,
				const VkDebugUtilsMessengerCallbackDataEXT * callback_data,
				void * user_data);

		vk::Result create_debug_utils_messenger_ext(
				vk::Instance instance,
				const vk::DebugUtilsMessengerCreateInfoEXT * create_info,
				const vk::AllocationCallbacks * allocator,
				vk::DebugUtilsMessengerEXT * debug_messenger);

		void destroy_debug_utils_messenger_ext(
				vk::Instance instance,
				vk::DebugUtilsMessengerEXT debug_messenger,
				const vk::AllocationCallbacks * allocator);


	public:
		explicit VulkanDebug(const std::shared_ptr<Instance>& instance, bool verbose_mode = false);
		~VulkanDebug();
	};
}



#endif //VALCANO_DEBUG_HPP
