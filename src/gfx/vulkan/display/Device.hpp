//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_DEVICE_HPP
#define VALCANO_DEVICE_HPP

#include <optional>
#include "Instance.hpp"
#include "Surface.hpp"

namespace mt::gfx::mtvk {
	class Device {
		struct QueueFamilyIndices{
			std::optional<uint32_t> graphics_family;
            std::optional<uint32_t> present_family;

			bool is_complete() {
				return graphics_family.has_value() && present_family.has_value();
			}
		};

        const std::vector<const char*> device_extensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

		std::shared_ptr<Instance> instance;

		vk::PhysicalDevice physical_device;
		vk::Device device;

        vk::PhysicalDeviceFeatures features;
        vk::PhysicalDeviceProperties properties;

        QueueFamilyIndices indices;

        vk::Queue graphics_queue;
		vk::Queue present_queue;

		vk::PhysicalDevice select_best_physical_device(std::vector<vk::PhysicalDevice> physical_devices,  const vk::SurfaceKHR& surface);

		int rate_physical_device(const vk::PhysicalDevice& device);

		bool is_physical_device_suitable(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

        vk::Device create_logical_device(const vk::PhysicalDevice &physical_device, const std::shared_ptr<Instance> &instance,  const vk::SurfaceKHR& surface);

        QueueFamilyIndices find_queue_families(const vk::PhysicalDevice& device,  const vk::SurfaceKHR& surface);

    public:
		Device(const std::shared_ptr<Instance> &instance, const std::shared_ptr<Surface>& surface);
		~Device() = default;

		void destroy();

        bool check_device_extension_support(const vk::PhysicalDevice &device);

        vk::Device get_device();
        vk::PhysicalDevice get_physical_device();

        QueueFamilyIndices get_queue_indices();

        void wait_till_idle();
    };
}



#endif //VALCANO_DEVICE_HPP
