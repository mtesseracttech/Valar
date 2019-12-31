//
// Created by MTesseracT on 30/12/2019.
//

#ifndef VALCANO_CONTEXTWINDOW_HPP
#define VALCANO_CONTEXTWINDOW_HPP


#include <iostream>
#include <string>
#include "GLFW/glfw3.h"

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
	};
}



#endif //VALCANO_CONTEXTWINDOW_HPP
