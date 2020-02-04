//
// Created by mtesseract on 03-01-20.
//

#include <aux/logging/Logger.hpp>
#include "WindowManager.hpp"

namespace mt::gfx{
    WindowManager::WindowManager(){
        glfwInit();
        Logger::log("Window Manager Initialized", Info);
    }

    WindowManager::~WindowManager() {
        if(main_window){
            callback_manager.deregister_window(main_window);
        }
        for(const auto& window : windows){
            callback_manager.deregister_window(window.second);
        }
        glfwTerminate();
        Logger::log("Window Manager Shut Down", Info);
    }

    void WindowManager::set_main_window(const std::shared_ptr<ContextWindow> &main_window) {
        Logger::log("Added Main Window: '" + main_window->get_title() + "' to Window Manager", Info);
        callback_manager.register_window(main_window);
    }

    void WindowManager::add_window(const std::shared_ptr<ContextWindow>& window) {
        Logger::log("Added Window to Window Manager: '" + window->get_title() + "'", Info);
        windows[window->get_title()] = window;
    }
}