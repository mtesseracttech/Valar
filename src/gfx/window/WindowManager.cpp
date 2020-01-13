//
// Created by mtesseract on 03-01-20.
//

#include <aux/logging/Logger.hpp>
#include "WindowManager.hpp"

namespace mt::gfx{
    WindowManager::WindowManager(){
        glfwInit();
        aux::Logger::log("Window Manager Initialized", aux::LogType::Info);
    }

    WindowManager::~WindowManager() {
        if(main_window){
            callback_manager.deregister_window(main_window);
        }
        for(const auto& window : windows){
            callback_manager.deregister_window(window.second);
        }
        glfwTerminate();
    }

    void WindowManager::set_main_window(const std::shared_ptr<ContextWindow> &main_window) {
        aux::Logger::log("Added Main Window: '" + main_window->get_title() + "' to Window Manager", aux::LogType::Info);
        callback_manager.register_window(main_window);
    }

    void WindowManager::add_window(const std::shared_ptr<ContextWindow>& window) {
        aux::Logger::log("Added Window to Window Manager: '" + window->get_title() + "'", aux::LogType::Info);
        windows[window->get_title()] = window;
    }
}