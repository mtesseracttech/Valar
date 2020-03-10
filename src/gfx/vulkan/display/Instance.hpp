//
// Created by MTesseracT on 31/12/2019.
//

#ifndef VALCANO_INSTANCE_HPP
#define VALCANO_INSTANCE_HPP

#include <aux/logging/Logger.hpp>
#include <util/GlfwHelper.hpp>

#include "vulkan/vulkan.hpp"

namespace mt::gfx::mtvk {
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

    bool check_validation_layer_support()
    {
        std::vector<vk::LayerProperties> available_layers = vk::enumerateInstanceLayerProperties();

        Logger::log(
            "Available Instance Layers: " + std::to_string(available_layers.size()),
            Info);
        for (const auto& layer : available_layers) {
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
                Logger::log(
                    "Could not find the validation layer: " + std::string(val_layer),
                    Error);
                return false;
            }
        }
        return true;
    }

    std::vector<const char*> get_required_extensions()
    {
        auto extensions = GlfwHelper::get_glfw_instance_extensions();

        if (enable_validation_layers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

public:
    explicit Instance(const std::string& app_name)
    {
        if (enable_validation_layers && !check_validation_layer_support()) {
            throw std::runtime_error(
                "Validation layers requested, but not available!");
        }

        vk::ApplicationInfo app_info;
        app_info.pApplicationName = app_name.c_str();
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Valcano";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;

        auto extensions = get_required_extensions();

        vk::InstanceCreateInfo create_info;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount = extensions.size();
        create_info.ppEnabledExtensionNames = extensions.data();

        if (enable_validation_layers) {
            create_info.enabledLayerCount = validation_layers.size();
            create_info.ppEnabledLayerNames = validation_layers.data();
            validation_layers_enabled = true;
        } else {
            create_info.enabledLayerCount = 0;
            validation_layers_enabled = false;
        }

        instance = vk::createInstance(create_info, nullptr);

        uint32_t instance_version = vk::enumerateInstanceVersion();

        std::stringstream inst_info;
        inst_info << "Vulkan Instance Version: "
                  << VK_VERSION_MAJOR(instance_version) << '.'
                  << VK_VERSION_MINOR(instance_version) << '.'
                  << VK_VERSION_PATCH(instance_version);
        Logger::log(inst_info, Info);

        std::vector<vk::ExtensionProperties> available_instance_extensions = vk::enumerateInstanceExtensionProperties();

        Logger::log("Available Instance Extensions: " + std::to_string(available_instance_extensions.size()),
            Info);
        for (const auto& extension : available_instance_extensions) {
            Logger::log("\t" + std::string(extension.extensionName), Info);
        }
    }

    ~Instance() = default;

    void destroy()
    {
        Logger::log("Destroyed Vulkan Instance", Info);
        instance.destroy();
    }

    bool has_validation_layers() const { return validation_layers_enabled; }

    std::vector<const char*> get_validation_layers() const
    {
        return validation_layers;
    }

    vk::Instance get_instance() const { return instance; }
};
} // namespace mt::gfx::mtvk

#endif // VALCANO_INSTANCE_HPP
