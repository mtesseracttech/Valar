//
// Created by MTesseracT on 30/12/2019.
//

#ifndef VALCANO_CONTEXTWINDOW_HPP
#define VALCANO_CONTEXTWINDOW_HPP


#include <iostream>
#include <string>
#include <memory>
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.hpp"

namespace mt::gfx{
	class ContextWindow : public std::enable_shared_from_this<ContextWindow>  {
	protected:
		int width = 0;
		int height = 0;
		GLFWwindow * window = nullptr;

	public:
		ContextWindow(uint32_t width, uint32_t height, const std::string& window_title);
		void framebuffer_resized(uint32_t width, uint32_t height);
		GLFWwindow * get_window();

		bool should_close();
		void process_events();

		uint32_t get_width();
		uint32_t get_height();

		vk::Extent2D get_extent();

	};
}



#endif //VALCANO_CONTEXTWINDOW_HPP
