//
// Created by MTesseracT on 31/12/2019.
//
#include "Instance.hpp"
#include <iostream>
#include <aux/logging/Logger.hpp>
#include <GLFW/glfw3.h>
#include <aux/exceptions/VulkanError.hpp>

namespace mt::gfx::mtvk {
	Instance::Instance(const std::string& app_name) {
        vk::ApplicationInfo app_info(app_name.c_str(),
                                     VK_MAKE_VERSION(1,0,0),
                                     "Valcano",
                                     VK_MAKE_VERSION(1,0,0),
                                     VK_API_VERSION_1_0
        );

        uint32_t glfw_ext_count = 0;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

        vk::InstanceCreateInfo create_info;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount =  glfw_ext_count;
        create_info.ppEnabledExtensionNames = glfw_extensions;
        create_info.enabledLayerCount = 0;

        if(vk::createInstance(&create_info, nullptr, &instance) != vk::Result::eSuccess){
            throw aux::VulkanError("Could not create a Vulkan Instance");
        }

        uint32_t instance_version = 0;
        vk::enumerateInstanceVersion(&instance_version);
        std::stringstream inst_info;
        inst_info << "Vulkan Instance Version: " << VK_VERSION_MAJOR(instance_version) <<
                  '.' << VK_VERSION_MINOR(instance_version) <<
                  '.' << VK_VERSION_PATCH(instance_version) << std::endl;
        aux::Logger::log(inst_info, aux::LogType::Info);

        uint32_t extension_count = 0;
        vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr, vk::DispatchLoaderStatic());
        std::vector<vk::ExtensionProperties> extensions(extension_count);
        vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data(), vk::DispatchLoaderStatic());

        aux::Logger::log("Available Instance Extensions: " + std::to_string(extension_count),aux::LogType::Info);
        for(const auto& ext : extensions){
            aux::Logger::log("\t" + std::string(ext.extensionName), aux::LogType::Info);
        }
	}

    Instance::~Instance() {
	    instance.destroy();
    }
}