//
// Created by mtesseract on 07-02-20.
//

#ifndef VALCANO_GLFWHELPER_HPP
#define VALCANO_GLFWHELPER_HPP

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

namespace mt {
class GlfwHelper {
public:
    static vk::SurfaceKHR get_glfw_surface(const vk::Instance& instance,
        GLFWwindow* window);
    static std::vector<const char*> get_glfw_instance_extensions();
};
} // namespace mt

#endif // VALCANO_GLFWHELPER_HPP
