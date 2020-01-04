//
// Created by MTesseracT on 30/12/2019.
//

#include <sstream>
#include <aux/logging/Logger.hpp>
#include "WindowCallbackManager.hpp"

namespace mt::gfx {
    void WindowCallbackManager::register_window(std::weak_ptr<gfx::ContextWindow> window) {
        if (!window.expired()) {
            auto win = window.lock();
            aux::Logger::log("Registering Callbacks for Window '" + win->get_title() + "'", aux::LogType::Info);
            glfwSetWindowUserPointer(win->get_window(), this);
            //Setting the general GLFW error callback
            glfwSetErrorCallback(window_error_callback);
            //Setting the GLFW FrameBuffer Resize callback, that is called whenever the window changes sizes
            glfwSetFramebufferSizeCallback(win->get_window(), framebuffer_resize_callback);
            //Cursor Position Callback
            glfwSetCursorPosCallback(win->get_window(), mouse_pos_callback);
            //Mouse Button Callback
            glfwSetMouseButtonCallback(win->get_window(), mouse_button_callback);
            //Keyboard Callback
            glfwSetKeyCallback(win->get_window(), keyboard_callback);
            window_callbacks[win->get_window()] = window;
        }
    }

    void WindowCallbackManager::deregister_window(std::weak_ptr<gfx::ContextWindow> window) {
        if (!window.expired()) {
            auto win = window.lock();
            aux::Logger::log("Deregistering Callbacks for Window '" + win->get_title() + "'", aux::LogType::Info);
            window_callbacks.erase(win->get_window());
        }
    }

    void WindowCallbackManager::framebuffer_resize_callback(GLFWwindow *window, int width, int height) {
        auto manager = get_callback_manager(window);
        if (manager) {
            auto it = manager.value().window_callbacks.find(window);
            if (it != manager.value().window_callbacks.end()) {
                auto wp = it->second;
                if (!wp.expired()) {
                    auto win = wp.lock();
                    win->framebuffer_resized(width, height);
                }
            }
        }
    }

    void WindowCallbackManager::window_error_callback(int error_code, const char *error_message) {
        std::stringstream ss;
        ss << "[GLFW-" << error_code << "] " << error_message;
        aux::Logger::log(ss, aux::LogType::Error);
    }

    void WindowCallbackManager::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto manager = get_callback_manager(window);
        if (manager) {
            //aux::Logger::log("Keyboard Event Handling Not Implemented Yet", aux::LogType::Info);
        }
    }

    void WindowCallbackManager::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        auto manager = get_callback_manager(window);
        if (manager) {
            //aux::Logger::log("Mouse Button Event Handling Not Implemented Yet", aux::LogType::Info);
        }
    }

    void WindowCallbackManager::mouse_pos_callback(GLFWwindow *window, double x, double y) {
        auto manager = get_callback_manager(window);
        if (manager) {
            //aux::Logger::log("Mouse Pos Event Handling Not Implemented Yet", aux::LogType::Info);
        }
    }

    std::optional<WindowCallbackManager> WindowCallbackManager::get_callback_manager(GLFWwindow *window) {
        if(window){
            auto manager = glfwGetWindowUserPointer(window);
            if(manager) return *reinterpret_cast<WindowCallbackManager *>(manager);
        }
        return {};
    }
}