//
// Created by MTesseracT on 2019-12-23.
//

#include "ValcanoApplication.hpp"
#include "gfx/window/WindowCallbackManager.hpp"
#include "io/filesystem/FileSystem.hpp"

#include "gfx/Renderer.hpp"

namespace mt {
	ValcanoApplication::ValcanoApplication(uint32_t win_width, uint32_t win_height, const std::string &app_name) {
        aux::Logger::log("Valcano application running from: " + io::get_execution_path());
        window_manager = std::make_shared<gfx::WindowManager>();
        main_window = std::make_shared<gfx::ContextWindow>(win_width, win_height, app_name);
        window_manager->set_main_window(main_window);
        renderer = std::make_shared<gfx::Renderer>(main_window, app_name);
	}

	void ValcanoApplication::run() {
		while(!main_window->should_close()){
			main_window->process_events();
			renderer->draw();
		}
		renderer->terminate();
	}
}

