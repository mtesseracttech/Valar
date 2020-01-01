//
// Created by MTesseracT on 2019-12-23.
//

#ifndef VALCANO_VALCANOAPPLICATION_HPP
#define VALCANO_VALCANOAPPLICATION_HPP

#include <iostream>
#include <gfx/Renderer.hpp>
#include "gfx/window/ContextWindow.hpp"

namespace mt{
	class ValcanoApplication {
		std::shared_ptr<gfx::ContextWindow> main_window;
		std::shared_ptr<gfx::Renderer> renderer;
	public:
		ValcanoApplication(uint32_t win_width, uint32_t win_height, const std::string& win_name);

		void initialize();
		void run();
	};
}


int main(int argc, const char** args){
	mt::ValcanoApplication app(800, 600,"Test");

	try {
		app.initialize();
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

}

#endif //VALCANO_VALCANOAPPLICATION_HPP