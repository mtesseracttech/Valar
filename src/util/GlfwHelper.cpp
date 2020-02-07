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
}