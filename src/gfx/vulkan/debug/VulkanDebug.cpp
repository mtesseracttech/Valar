//
// Created by MTesseracT on 02/01/2020.
//

#include <aux/logging/Logger.hpp>
#include <iostream>
#include <util/StringUtils.hpp>
#include "VulkanDebug.hpp"

namespace mt::gfx::mtvk {
	VulkanDebug::VulkanDebug(const std::shared_ptr<Instance>& instance, bool verbose_mode) {
		assert(instance);

		this->instance = instance;

		vk::DebugUtilsMessengerCreateInfoEXT create_info;
		create_info.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
									  vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
									  vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
		if(verbose_mode) create_info.messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
		create_info.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT ::eGeneral |
								  vk::DebugUtilsMessageTypeFlagBitsEXT ::eValidation |
								  vk::DebugUtilsMessageTypeFlagBitsEXT ::ePerformance;
		create_info.pfnUserCallback = vulkan_debug_callback;
		create_info.pUserData = this;

		if (create_debug_utils_messenger_ext(instance->get_instance(), &create_info, nullptr, &debug_messenger) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to set up debug messenger!");
		}

		aux::Logger::log("Set up Vulkan Debug Messenger", aux::LogType::Info);
	}

	vk::Result VulkanDebug::create_debug_utils_messenger_ext(vk::Instance instance,
			const vk::DebugUtilsMessengerCreateInfoEXT* create_info,
			const vk::AllocationCallbacks* allocator,
			vk::DebugUtilsMessengerEXT* debug_messenger) {

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) instance.getProcAddr("vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr) {
			return vk::Result(func(instance,
					reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>(create_info),
					reinterpret_cast<const VkAllocationCallbacks *>(allocator),
					reinterpret_cast<VkDebugUtilsMessengerEXT_T **>(debug_messenger)));
		} else {
			return vk::Result::eErrorExtensionNotPresent;
		}
	}

	void VulkanDebug::destroy_debug_utils_messenger_ext(vk::Instance instance,
	                                                    vk::DebugUtilsMessengerEXT debug_messenger,
	                                                    const vk::AllocationCallbacks * allocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance,
					static_cast<VkDebugUtilsMessengerEXT>(debug_messenger),
					reinterpret_cast<const VkAllocationCallbacks *>(allocator));
		}
	}

	vk::Bool32 VulkanDebug::vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT vk_message_severity,
	                                            VkDebugUtilsMessageTypeFlagsEXT vk_message_type,
	                                            const VkDebugUtilsMessengerCallbackDataEXT * vk_callback_data,
	                                            void * vk_user_data) {
		auto message_severity         = vk::DebugUtilsMessageSeverityFlagsEXT(vk_message_severity);
		auto message_type             = vk::DebugUtilsMessageTypeFlagsEXT(vk_message_type);
		auto message_callback_data    = vk::DebugUtilsMessengerCallbackDataEXT(*vk_callback_data);

        (void)vk_user_data; //To get the compiler to stop complaining about an unused variable.

		aux::LogType log_level = aux::LogType::Info;
		if((uint32_t)message_severity >= (uint32_t)vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning){
			log_level = aux::LogType ::Error;
		}

		std::string severity_string = vk::to_string(message_severity);
		severity_string = util::string::to_upper(severity_string.substr(2, severity_string.size() - 4));

		std::string type_string = vk::to_string(message_type);
		type_string = util::string::to_upper(type_string.substr(2, type_string.size() - 4));

		std::stringstream message;
		message << "[VK-" << type_string << "][" << severity_string << "]:";

		aux::Logger::log(message, log_level);
		aux::Logger::log(message_callback_data.pMessage, log_level);

		return false;
	}

	VulkanDebug::~VulkanDebug() {
		aux::Logger::log("Destroyed Vulkan Debug Messenger", aux::LogType::Info);
		destroy_debug_utils_messenger_ext(instance->get_instance(), debug_messenger, nullptr);
	}
}