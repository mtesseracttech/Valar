//
// Created by mtesseract on 03-01-20.
//
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <aux/logging/Logger.hpp>
#include "Surface.hpp"

namespace mt::gfx::mtvk {


    Surface::Surface(const std::shared_ptr<Instance>& instance, const ContextWindow& window): instance(instance) {
        if(glfwCreateWindowSurface(instance->get_instance(), window.get_window(), nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface))){
            throw std::runtime_error("Failed to create window surface!");
        }
        aux::Logger::log("Created the Window Surface", aux::LogType::Info);
    }

    vk::SurfaceKHR Surface::get_surface() const {
        return surface;
    }

    void Surface::destroy() {
        instance->get_instance().destroySurfaceKHR(surface);
        aux::Logger::log("Destroyed the Window Surface", aux::LogType::Info);
    }
}