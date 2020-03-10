//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_WINDOWMANAGER_HPP
#define VALCANO_WINDOWMANAGER_HPP

#include "ContextWindow.hpp"
#include "WindowCallbackManager.hpp"

namespace mt::gfx {
class WindowManager {
    std::shared_ptr<ContextWindow> main_window;
    std::map<std::string, std::shared_ptr<ContextWindow>> windows;
    WindowCallbackManager callback_manager;

public:
    WindowManager();
    ~WindowManager();

    void set_main_window(const std::shared_ptr<ContextWindow>& main_window);

    void add_window(const std::shared_ptr<ContextWindow>& window);
};
} // namespace mt::gfx

#endif // VALCANO_WINDOWMANAGER_HPP
