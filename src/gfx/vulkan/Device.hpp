//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_DEVICE_HPP
#define VALCANO_DEVICE_HPP

#include <optional>
#include "Instance.hpp"

namespace mt::gfx::mtvk {
	class Device {
		struct QueueFamilyIndices{
			std::optional<uint32_t> graphics_family;

			bool is_complete() {
				return graphics_family.has_value();
			}
		};

		std::shared_ptr<Instance> instance;

		vk::PhysicalDevice physical_device;
		vk::Device device;

		vk::PhysicalDeviceFeatures features;
		vk::PhysicalDeviceProperties properties;

		vk::PhysicalDevice select_best_physical_device(std::vector<vk::PhysicalDevice> physical_devices);
		int rate_physical_device(const vk::PhysicalDevice& device);
		bool is_physical_device_suitable(const vk::PhysicalDevice& device);
		QueueFamilyIndices find_queue_families(const vk::PhysicalDevice& device);
	public:
		Device(const std::shared_ptr<Instance> &instance);
		~Device();
	};
}



#endif //VALCANO_DEVICE_HPP
