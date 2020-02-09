//
// Created by mtesseract on 07-02-20.
//

#include "GlfwHelper.hpp"
#include <GLFW/glfw3.h>

namespace mt{
    vk::SurfaceKHR GlfwHelper::get_glfw_surface(const vk::Instance &instance, GLFWwindow *window) {
        vk::SurfaceKHR surface;
        if(glfwCreateWindowSurface(instance, window, nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface))){
            throw std::runtime_error("Failed to create window surface!");
        }
        return surface;
    }

	std::vector<const char *> GlfwHelper::get_glfw_instance_extensions() {
		uint32_t glfw_ext_count = 0;
		auto glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
		return std::vector<const char*> (glfw_extensions, glfw_extensions + glfw_ext_count);
    }
}