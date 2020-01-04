//
// Created by MTesseracT on 30/12/2019.
//

#include <sstream>
#include <aux/logging/Logger.hpp>
#include "WindowCallbackManager.hpp"

namespace mt::gfx{
	void WindowCallbackManager::register_window(std::weak_ptr<gfx::ContextWindow> window) {
	    if(!window.expired()){
			auto win = window.lock();
            aux::Logger::log("Registering Callbacks for Window '" + win->get_title() + "'", aux::LogType::Info);
            glfwSetWindowUserPointer(win->get_window(), this);
			//Setting the general GLFW error callback
            glfwSetErrorCallback(window_error_callback);
            //Setting the GLFW FrameBuffer Resize callback, that is called whenever the window changes sizes
            glfwSetFramebufferSizeCallback(win->get_window(), fb_resize_callback);
			window_callbacks[win->get_window()] = window;
		}
	}

    void WindowCallbackManager::deregister_window(std::weak_ptr<gfx::ContextWindow> window) {
        if(!window.expired()) {
            auto win = window.lock();
            aux::Logger::log("Deregistering Callbacks for Window '" + win->get_title() + "'", aux::LogType::Info);
            window_callbacks.erase(win->get_window());
        }
    }

    void WindowCallbackManager::fb_resize_callback(GLFWwindow *window, int width, int height) {
        if(window){
            auto callback_manager = (WindowCallbackManager*)glfwGetWindowUserPointer(window);
            auto it = callback_manager->window_callbacks.find(window);
            if(it != callback_manager->window_callbacks.end()){
                auto wp = it->second;
                if(!wp.expired()){
                    auto win = wp.lock();
                    win->framebuffer_resized(width, height);
                }
            }
        }
    }

    void WindowCallbackManager::window_error_callback(int error_code, const char * error_message) {

	    std::stringstream ss;
	    ss << "[GLFW-" << error_code << "] " << error_message;
        aux::Logger::log(ss, aux::LogType::Error);
    }

    WindowCallbackManager::WindowCallbackManager() = default;

    WindowCallbackManager::~WindowCallbackManager() = default;
}