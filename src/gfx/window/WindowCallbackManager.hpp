//
// Created by MTesseracT on 30/12/2019.
//

#ifndef VALCANO_WINDOWCALLBACKMANAGER_HPP
#define VALCANO_WINDOWCALLBACKMANAGER_HPP

#include <memory>
#include <map>
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.hpp"
#include "gfx/window/ContextWindow.hpp"

namespace mt::gfx{
	class WindowCallbackManager {
	    std::map<GLFWwindow*, std::weak_ptr<gfx::ContextWindow>> window_callbacks;
		//std::map<GLFWWindow*, std::shared_ptr<InputManager>> mouse_callback;
		//std::map<GLFWWindow*, std::shared_ptr<InputManager>> keyboard_callback;
		static void fb_resize_callback(GLFWwindow* window, int width, int height);

		static void window_error_callback(int, const char *);
	public:
	    WindowCallbackManager();
	    ~WindowCallbackManager();

	    void register_window(std::weak_ptr<gfx::ContextWindow> window);
	    void deregister_window(std::weak_ptr<gfx::ContextWindow> window);
	};
}


#endif //VALCANO_WINDOWCALLBACKMANAGER_HPP
