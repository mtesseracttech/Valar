//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_DEVICE_HPP
#define VALCANO_DEVICE_HPP

#include <optional>
#include <set>
#include <queue>
//#include "Instance.hpp"
#include "Surface.hpp"
#include "aux/logging/Logger.hpp"

namespace mt::gfx::mtvk {
    namespace ext {
        const std::vector<const char *> device_extensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    }

	class Device {
		struct QueueFamilyIndices{
			std::optional<uint32_t> graphics_family;
            std::optional<uint32_t> present_family;

			bool is_complete() {
				return graphics_family.has_value() && present_family.has_value();
			}
		};

		std::shared_ptr<Surface> surface;

		vk::PhysicalDevice physical_device;
		vk::Device device;

        vk::PhysicalDeviceFeatures features;
        vk::PhysicalDeviceProperties properties;

        vk::Queue graphics_queue;
		vk::Queue present_queue;

		QueueFamilyIndices indices;

    public:
        explicit Device(const std::shared_ptr<Surface>& surface) : surface(surface){}

        ~Device() = default;

        void create(const Instance &instance){
            auto best_physical_device = select_best_physical_device(instance.get_instance().enumeratePhysicalDevices(), surface->get_surface());
            auto logical_device = create_logical_device(best_physical_device, instance, surface->get_surface());

            physical_device = best_physical_device;
            device = logical_device;
            features = best_physical_device.getFeatures();
            properties = best_physical_device.getProperties();
            indices = find_queue_families(physical_device, surface->get_surface());

            Logger::log("Set up Vulkan Device", Info);

            graphics_queue = device.getQueue(indices.graphics_family.value(),0);
            present_queue = device.getQueue(indices.present_family.value(), 0);

            Logger::log("Set up Graphics and Present Queues", Info);
        }

        //Call this to destroy the vulkan device, at the very end of all vulkan activity
        void destroy(){
            Logger::log("Destroyed Vulkan Device",Info);
            if(device) device.destroy();
        }

        //Wait till device is idle to load resources/close the device properly
        void wait_till_idle(){
            device.waitIdle();
        }

        //Returns the command queue indices
        QueueFamilyIndices get_queue_indices() const {
            return indices;
        }

        //Gets the physical device (contains capabilities, specs, etc.)
        vk::PhysicalDevice get_physical_device() const {
            return physical_device;
        }

        //Gets the logical device (used for most vulkan functionality)
        vk::Device get_device() const {
            return device;
        }

        //The Command Queues to which to submit commandbuffers to.
        vk::Queue get_graphics_queue() const{
            return graphics_queue;
        }

        vk::Queue get_present_queue() const{
            return present_queue;
        }

	private:
		static vk::PhysicalDevice select_best_physical_device(std::vector<vk::PhysicalDevice> physical_devices,  const vk::SurfaceKHR& surface){
            if (physical_devices.empty()) {
                throw std::runtime_error("Failed to find GPUs with Vulkan support!");
            }

            std::priority_queue<std::pair<int, vk::PhysicalDevice>> ranked_devices;

            for (const auto & device : physical_devices) {
                if (is_physical_device_suitable(device, surface)) {
                    //ranked_devices.push(std::make_pair(rate_physical_device(device), device));
                    ranked_devices.emplace(rate_physical_device(device), device);
                }
            }

            Logger::log("Found the following suitable GPUs: ", Info);
            if(ranked_devices.empty()){
                Logger::log("None", Error);
                throw std::runtime_error("Failed to find a device that satisfies all hard requirements!");
            }

            auto best_device = ranked_devices.top().second;
            int i = 1;
            while(!ranked_devices.empty()){
                auto ranked_device = ranked_devices.top();
                std::stringstream ss;
                ss << i++ << ". " << ranked_device.second.getProperties().deviceName << " (" << ranked_device.first << ")";
                Logger::log(ss, Info);
                ranked_devices.pop();
            }

            Logger::log("Selected the GPU: " + std::string(best_device.getProperties().deviceName), Info);

            return best_device;
		}

		static int rate_physical_device(const vk::PhysicalDevice& device){
            auto device_properties = device.getProperties();
            auto device_features = device.getFeatures();

            int score = 0;

            if (device_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
                score += 1000;
            }

            score += device_properties.limits.maxImageDimension2D;

            if (!device_features.geometryShader) {
                score /= 10;
            }

            return score;
		}

		static bool is_physical_device_suitable(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR& surface){
            auto queue_indices = find_queue_families(physical_device, surface);

            bool extensions_supported = check_device_extension_support(physical_device);

            bool swapchain_adequate = false;
            if(extensions_supported){
                //auto capabilities = physical_device.getSurfaceCapabilitiesKHR(surface);
                auto formats = physical_device.getSurfaceFormatsKHR(surface);
                auto present_modes = physical_device.getSurfacePresentModesKHR(surface);
                swapchain_adequate = !formats.empty() && !present_modes.empty();
            }

            return queue_indices.is_complete() && extensions_supported && swapchain_adequate;
		}

        static vk::Device create_logical_device(const vk::PhysicalDevice &physical_device, const Instance& instance,  const vk::SurfaceKHR& surface){
            auto indices = find_queue_families(physical_device, surface);

            std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
            std::set<uint32_t> unique_queue_families = {indices.graphics_family.value(), indices.present_family.value()};

            float queue_priority = 1.0f;

            for (uint32_t queue_family : unique_queue_families) {
                vk::DeviceQueueCreateInfo queue_create_info;
                queue_create_info.queueFamilyIndex = queue_family;
                queue_create_info.queueCount = 1;
                queue_create_info.pQueuePriorities = &queue_priority;
                queue_create_infos.push_back(queue_create_info);
            }

            vk::PhysicalDeviceFeatures device_features;

            vk::DeviceCreateInfo create_info;
            create_info.queueCreateInfoCount = queue_create_infos.size();
            create_info.pQueueCreateInfos = queue_create_infos.data();
            create_info.pEnabledFeatures = &device_features;
            create_info.enabledExtensionCount = ext::device_extensions.size();
            create_info.ppEnabledExtensionNames = ext::device_extensions.data();

            auto validation_layers = instance.get_validation_layers();

            if (instance.has_validation_layers()) {
                create_info.enabledLayerCount = validation_layers.size();
                create_info.ppEnabledLayerNames = validation_layers.data();
            } else {
                create_info.enabledLayerCount = 0;
            }

            return physical_device.createDevice(create_info);
		}

        static QueueFamilyIndices find_queue_families(const vk::PhysicalDevice& physical_device,  const vk::SurfaceKHR& surface){
            QueueFamilyIndices indices;
            auto queue_families = physical_device.getQueueFamilyProperties();
            int i = 0;
            for (const auto& queue : queue_families) {
                if (queue.queueFlags & vk::QueueFlagBits::eGraphics) {
                    indices.graphics_family = i;
                }
                if (physical_device.getSurfaceSupportKHR(i, surface)) {
                    indices.present_family = i;
                }
                if(indices.is_complete()){
                    break;
                }
                i++;
            }

            return indices;
        }

        QueueFamilyIndices find_queue_families(){
		    assert(physical_device && surface);
		    return find_queue_families(physical_device, surface->get_surface());
        }

        static bool check_device_extension_support(const vk::PhysicalDevice &device){
            auto available_extensions = device.enumerateDeviceExtensionProperties();
            std::set<std::string> required_extensions(ext::device_extensions.begin(), ext::device_extensions.end());

            for (const auto& extension : available_extensions) {
                required_extensions.erase(extension.extensionName);
            }

            return required_extensions.empty();
        }
    };
}



#endif //VALCANO_DEVICE_HPP
