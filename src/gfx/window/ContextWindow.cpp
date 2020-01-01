//
// Created by MTesseracT on 30/12/2019.
//

#include <aux/logging/Logger.hpp>
#include "ContextWindow.hpp"
#include "aux/CallbackManager.hpp"
#include "vulkan/vulkan.hpp"
#include "GLFW/glfw3.h"

namespace mt::gfx{

	ContextWindow::ContextWindow(uint32_t width, uint32_t height, const std::string &window_title) {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), window_title.c_str(), nullptr, nullptr);

		if(!window) {
			const char* error_description;
			int glfw_error_code = glfwGetError(&error_description);
			if (error_description){
				throw std::runtime_error("GLFW Window Creation Error: \n" + std::string(error_description));
			}
			throw std::runtime_error("Could not Create a GLFW Window, code: " + std::to_string(glfw_error_code));
		}

		glfwGetWindowSize(window, &this->width, &this->height);

		aux::Logger::log("Window initialized with a size of: " + std::to_string(this->width) + "x" + std::to_string(this->height));
	}

	GLFWwindow *ContextWindow::get_window() {
		return window;
	}

	void ContextWindow::framebuffer_resized(uint32_t new_width, uint32_t new_height) {
        width = static_cast<int>(new_width);
        height = static_cast<int>(new_height);
        aux::Logger::log("Framebuffer Resized to: " + std::to_string(width) + "x" + std::to_string(height));
    }

	bool ContextWindow::should_close() {
		if(window){
			return glfwWindowShouldClose(window);
		}
		else return true;
	}

	void ContextWindow::process_events() {
		if(window){
			return glfwPollEvents();
		}
	}

    uint32_t ContextWindow::get_width() {
        return static_cast<uint32_t>(width);
    }

    uint32_t ContextWindow::get_height() {
        return static_cast<uint32_t>(height);
    }

    vk::Extent2D ContextWindow::get_extent() {
        return {get_width(), get_height()};
    }
}