//
// Created by MTesseracT on 30/12/2019.
//

#include <sstream>
#include <aux/logging/Logger.hpp>
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
			//Setting the general GLFW error callback
            glfwSetErrorCallback(window_error_callback);
            //Setting the GLFW FrameBuffer Resize callback, that is called whenever the window changes sizes
            glfwSetFramebufferSizeCallback(win->get_window(), fb_resize_callback);
			window_callbacks[win->get_window()] = window;
		}
	}

    void CallbackManager::deregister_window(std::weak_ptr<gfx::ContextWindow> window) {
        if(!window.expired()) {
            auto win = window.lock()->get_window();
            window_callbacks.erase(win);
        }
    }

    void CallbackManager::window_error_callback(int error_code, const char * error_message) {
	    std::stringstream ss;
	    ss << "[GLFW] ";
	    switch(error_code){
	        case GLFW_NOT_INITIALIZED: ss << "GLFW has not been initialized."; break;
	        case GLFW_NO_CURRENT_CONTEXT: ss << "No context is current for this thread."; break;
	        case GLFW_INVALID_ENUM: ss << "One of the enum parameters for the function was given an invalid enum."; break;
	        case GLFW_INVALID_VALUE: ss << "One of the parameters for the function was given an invalid value."; break;
	        case GLFW_OUT_OF_MEMORY: ss << "A memory allocation failed. "; break;
	        case GLFW_API_UNAVAILABLE: ss << "GLFW could not find support for the requested client API on the system. "; break;
	        case GLFW_VERSION_UNAVAILABLE: ss << "The requested client API version is not available."; break;
	        case GLFW_PLATFORM_ERROR: ss << "A platform-specific error occurred that does not match any of the more specific categories."; break;
	        case GLFW_FORMAT_UNAVAILABLE: ss << "The clipboard did not contain data in the requested format."; break;
	        default: ss << "Unknown Error";
        }
        aux::Logger::log(ss, aux::LogType::Error);
    }
}