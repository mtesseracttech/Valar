//
// Created by MTesseracT on 30/12/2019.
//

#include "CallbackManager.hpp"

namespace mt::aux{

	std::map<GLFWwindow*, std::weak_ptr<gfx::ContextWindow>> CallbackManager::window_callbacks;

	void CallbackManager::fb_resize_callback(GLFWwindow *window, int width, int height) {
		auto it = window_callbacks.find(window);
		if(it != window_callbacks.end()){
			auto wp = it->second;
			if(!wp.expired()){
				auto win = wp.lock();
				win->framebuffer_resized(width, height);
			}
		}
	}

	void CallbackManager::register_window(std::weak_ptr<gfx::ContextWindow> window) {
		if(!window.expired()){
			auto win = window.lock();
			glfwSetFramebufferSizeCallback(win->get_window(), fb_resize_callback);
			window_callbacks[win->get_window()] = window;
		}
	}
}