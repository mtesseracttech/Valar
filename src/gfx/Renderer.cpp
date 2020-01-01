//
// Created by MTesseracT on 31/12/2019.
//

#include "Renderer.hpp"
namespace mt::gfx {

	Renderer::Renderer(std::weak_ptr<ContextWindow> render_window) : window(render_window){
	    instance = std::make_shared<mtvk::Instance>();
	}
}