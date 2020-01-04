//
// Created by MTesseracT on 30/12/2019.
//

#ifndef VALCANO_WINDOWCALLBACKMANAGER_HPP
#define VALCANO_WINDOWCALLBACKMANAGER_HPP

#include <memory>
#include <optional>
#include <map>
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.hpp"
#include "gfx/window/ContextWindow.hpp"

namespace mt::gfx{
	class WindowCallbackManager {
	    std::map<GLFWwindow*, std::weak_ptr<gfx::ContextWindow>> window_callbacks;
		//std::map<GLFWWindow*, std::weak_ptr<InputManager>> mouse_callback;
		//std::map<GLFWWindow*, std::weak_ptr<InputManager>> keyboard_callback;
		static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
        static void mouse_pos_callback(GLFWwindow* window,double x,double y);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static std::optional<WindowCallbackManager> get_callback_manager(GLFWwindow* window);

		static void window_error_callback(int, const char *);
	public:
	    WindowCallbackManager() = default;
	    ~WindowCallbackManager() = default;

	    void register_window(std::weak_ptr<gfx::ContextWindow> window);
	    void deregister_window(std::weak_ptr<gfx::ContextWindow> window);
	};
}


#endif //VALCANO_WINDOWCALLBACKMANAGER_HPP
