//
// Created by MTesseracT on 31/12/2019.
//

#ifndef VALCANO_RENDERER_HPP
#define VALCANO_RENDERER_HPP

#include <gfx/vulkan/Instance.hpp>
#include <memory>
#include <gfx/window/ContextWindow.hpp>

namespace mt::gfx{
	class Renderer {
		std::weak_ptr<ContextWindow> window;
		std::shared_ptr<mtvk::Instance> instance;

	public:
		Renderer(std::weak_ptr<ContextWindow> render_window, const std::string& app_name);
	};
}



#endif //VALCANO_RENDERER_HPP
