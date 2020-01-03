//
// Created by MTesseracT on 02/01/2020.
//

#include <queue>
#include <aux/logging/Logger.hpp>
#include <iostream>
#include "Device.hpp"

namespace mt::gfx::mtvk {
	Device::Device(const std::shared_ptr<Instance>& instance) {
		physical_device = select_best_physical_device(instance->get_instance().enumeratePhysicalDevices());
		features = physical_device.getFeatures();
		properties = physical_device.getProperties();

		aux::Logger::log("Selected the GPU: " + std::string(properties.deviceName), aux::LogType::Info);
	}

	Device::~Device(){
		
	}

	vk::PhysicalDevice Device::select_best_physical_device(std::vector<vk::PhysicalDevice> physical_devices) {
		if (physical_devices.empty()) {
			throw std::runtime_error("Failed to find GPUs with Vulkan support!");
		}

		std::priority_queue<std::pair<int, vk::PhysicalDevice>> ranked_devices;

		for (const auto & device : physical_devices) {
			if (is_physical_device_suitable(device)) {
				ranked_devices.push(std::make_pair(rate_physical_device(device), device));
			}
		}

		aux::Logger::log("Found the following suitable GPUs: ", aux::LogType::Info);
		if(ranked_devices.empty()){
			aux::Logger::log("None", aux::LogType::Error);
			throw std::runtime_error("Failed to find a device that satisfies all hard requirements!");
		}

		auto best_device = ranked_devices.top().second;
		int i = 1;
		while(!ranked_devices.empty()){
			auto ranked_device = ranked_devices.top();
			std::stringstream ss;
			ss << i++ << ". " << ranked_device.second.getProperties().deviceName << " (" << ranked_device.first << ")";
			aux::Logger::log(ss, aux::LogType::Info);
			ranked_devices.pop();
		}

		return best_device;
	}

	int Device::rate_physical_device(const vk::PhysicalDevice & device) {
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

	Device::QueueFamilyIndices Device::find_queue_families(const vk::PhysicalDevice& device) {
		QueueFamilyIndices indices;
		auto queue_families = device.getQueueFamilyProperties();
		int i = 0;
		for (const auto& queue : queue_families) {
			if (queue.queueFlags & vk::QueueFlagBits::eGraphics) {
				indices.graphics_family = i;
			}
			if(indices.is_complete()){
				break;
			}
			i++;
		}

		return indices;
	}

	bool Device::is_physical_device_suitable(const vk::PhysicalDevice& device){
		auto indices = find_queue_families(device);

		return indices.is_complete();
	}
}