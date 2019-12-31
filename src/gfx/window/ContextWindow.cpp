//
// Created by MTesseracT on 30/12/2019.
//

#include "ContextWindow.hpp"
#include "aux/CallbackManager.hpp"
#include "vulkan/vulkan.hpp"

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


	}

	GLFWwindow *ContextWindow::get_window() {
		return window;
	}

	void ContextWindow::framebuffer_resized(uint32_t width, uint32_t height) {
		std::cout << "Framebuffer Resized to: " << width << "x" << height << std::endl;
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
}