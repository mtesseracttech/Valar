//
// Created by MTesseracT on 31/12/2019.
//
#include "Instance.hpp"
#include <iostream>
#include <aux/logging/Logger.hpp>
#include <GLFW/glfw3.h>
#include <aux/exceptions/VulkanError.hpp>

namespace mt::gfx::mtvk {
	Instance::Instance(const std::string &app_name) {
		if (enable_validation_layers && !check_validation_layer_support()) {
			throw std::runtime_error("Validation layers requested, but not available!");
		}

		vk::ApplicationInfo app_info;
		app_info.pApplicationName      = app_name.c_str();
		app_info.applicationVersion    = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName           = "Valcano";
		app_info.engineVersion         = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion            = VK_API_VERSION_1_0;

		auto extensions = get_required_extensions();

		vk::InstanceCreateInfo create_info;
		create_info.pApplicationInfo           = &app_info;
		create_info.enabledExtensionCount      = extensions.size();
		create_info.ppEnabledExtensionNames    = extensions.data();

		if (enable_validation_layers) {
			create_info.enabledLayerCount      = validation_layers.size();
			create_info.ppEnabledLayerNames    = validation_layers.data();
			validation_layers_enabled          = true;
		} else {
			create_info.enabledLayerCount      = 0;
			validation_layers_enabled          = false;
		}

		instance = vk::createInstance(create_info, nullptr);

		uint32_t instance_version = vk::enumerateInstanceVersion();

		std::stringstream inst_info;
		inst_info << "Vulkan Instance Version: " << VK_VERSION_MAJOR(instance_version) <<
		          '.' << VK_VERSION_MINOR(instance_version) <<
		          '.' << VK_VERSION_PATCH(instance_version);
		Logger::log(inst_info, Info);

		std::vector<vk::ExtensionProperties> available_instance_extensions = vk::enumerateInstanceExtensionProperties();

		Logger::log("Available Instance Extensions: " + std::to_string(available_instance_extensions.size()), Info);
		for (const auto &extension : available_instance_extensions) {
			Logger::log("\t" + std::string(extension.extensionName), Info);
		}
	}

    void Instance::destroy() {
        Logger::log("Destroyed Vulkan Instance", Info);
        instance.destroy();
    }

	bool Instance::check_validation_layer_support() {
		std::vector<vk::LayerProperties> available_layers = vk::enumerateInstanceLayerProperties();

		Logger::log("Available Instance Layers: " + std::to_string(available_layers.size()), Info);
		for (const auto &layer : available_layers) {
			Logger::log("\t" + std::string(layer.layerName), Info);
		}

		for (const auto val_layer : validation_layers) {
			bool layerFound = false;

			for (const auto& layer_properties : available_layers) {
				if (strcmp(val_layer, layer_properties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
                Logger::log("Could not find the validation layer: " + std::string(val_layer), Error);
				return false;
			}
		}
		return true;
	}

	std::vector<const char *> Instance::get_required_extensions() {
		uint32_t glfw_ext_count = 0;
		auto glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

		std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_ext_count);

		if (enable_validation_layers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool Instance::has_validation_layers() const {
		return validation_layers_enabled;
	}

    std::vector<const char *> Instance::get_validation_layers() const {
        return validation_layers;
    }

    vk::Instance Instance::get_instance() const{
        return instance;
    }
}