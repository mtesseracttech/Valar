//
// Created by MTesseracT on 31/12/2019.
//
#include "Instance.hpp"
#include <iostream>
#include <aux/logging/Logger.hpp>
#include "vulkan/vulkan.hpp"

namespace mt::gfx::mtvk {
	Instance::Instance() {
		uint32_t instance_version = 0;
		vk::Result result = vk::enumerateInstanceVersion(&instance_version);
		uint32_t major = VK_VERSION_MAJOR(instance_version);
		uint32_t minor = VK_VERSION_MINOR(instance_version);
		uint32_t patch = VK_VERSION_PATCH(instance_version);

		aux::Logger::log("Vulkan Instance Version: " + std::to_string(major) + '.' +
		std::to_string(minor) + '.' +
		std::to_string(patch) + "\n");

		uint32_t extension_count = 0;
		vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr, vk::DispatchLoaderStatic());
		std::vector<vk::ExtensionProperties> extensions(extension_count);
		vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data(), vk::DispatchLoaderStatic());

		aux::Logger::log("Available Instance Extensions: " + std::to_string(extension_count));
		for(const auto& ext : extensions){
			aux::Logger::log(std::string(ext.extensionName) + " - v" + std::to_string(ext.specVersion));
		}
	}
}