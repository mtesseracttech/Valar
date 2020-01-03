//
// Created by mtesseract on 03-01-20.
//

#include <aux/logging/Logger.hpp>
#include "WindowManager.hpp"

namespace mt::gfx{
    WindowManager::WindowManager(std::shared_ptr<ContextWindow> main_window) : main_window(main_window){
        aux::Logger::log("Window Manager Initialized and Added Main Window: '" + main_window->get_title() + "'", aux::LogType::Info);
        callback_manager.register_window(main_window);
    }

    WindowManager::~WindowManager() {
        if(main_window){
            aux::Logger::log("Window Manager Removed Main Window: '" + main_window->get_title() + "'", aux::LogType::Info);
            callback_manager.deregister_window(main_window);
        }
        for(const auto& window : windows){
            aux::Logger::log("Window Manager Removed Window: '" + window.second->get_title() + "'", aux::LogType::Info);
            callback_manager.deregister_window(window.second);
        }
    }

    void WindowManager::add_window(std::shared_ptr<ContextWindow> window) {
        aux::Logger::log("Added Window to Window Manager: '" + window->get_title() + "'", aux::LogType::Info);
        windows[window->get_title()] = window;
    }
}