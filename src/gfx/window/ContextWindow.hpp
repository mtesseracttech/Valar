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
    class Renderer;

	class ContextWindow : public std::enable_shared_from_this<ContextWindow>  {
	protected:
		int width = 0;

		int height = 0;

		GLFWwindow * window = nullptr;

		std::string title;

		std::weak_ptr<Renderer> renderer;

	public:
		ContextWindow(uint32_t width, uint32_t height, const std::string& window_title);
		~ContextWindow();

		void framebuffer_resized(uint32_t width, uint32_t height);

		bool should_close();

		void process_events();

        GLFWwindow * get_window() const;

		uint32_t get_width() const;
		uint32_t get_height() const;

		vk::Extent2D get_extent() const;

		std::string get_title() const;
		void set_title(const std::string& title);

		void set_renderer(const std::weak_ptr<Renderer>& renderer);
	};
}



#endif //VALCANO_CONTEXTWINDOW_HPP
