//
// Created by mtesseract on 07-02-20.
//

#ifndef VALCANO_GLFWHELPER_HPP
#define VALCANO_GLFWHELPER_HPP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace mt{
    class GlfwHelper {
    public:
        static vk::SurfaceKHR get_glfw_surface(const vk::Instance& instance, GLFWwindow * window);
    };
}



#endif //VALCANO_GLFWHELPER_HPP
