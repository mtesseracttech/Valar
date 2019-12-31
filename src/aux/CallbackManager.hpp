//
// Created by MTesseracT on 30/12/2019.
//

#ifndef VALCANO_CALLBACKMANAGER_HPP
#define VALCANO_CALLBACKMANAGER_HPP

#include <memory>
#include <map>
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.hpp"
#include "gfx/window/ContextWindow.hpp"

namespace mt::aux{
	class CallbackManager {
		static std::map<GLFWwindow*, std::weak_ptr<gfx::ContextWindow>> window_callbacks;
		//std::map<GLFWWindow*, std::shared_ptr<InputManager>> mouse_callback;
		//std::map<GLFWWindow*, std::shared_ptr<InputManager>> keyboard_callback;


		static void fb_resize_callback(GLFWwindow* window, int width, int height);

	public:
		static void register_window(std::weak_ptr<gfx::ContextWindow> window);
	};
}


#endif //VALCANO_CALLBACKMANAGER_HPP
