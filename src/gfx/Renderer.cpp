//
// Created by MTesseracT on 31/12/2019.
//

#include "Renderer.hpp"
namespace mt::gfx {

	Renderer::Renderer(const std::weak_ptr<ContextWindow>& render_window, const std::string& app_name) : window(render_window){
	    instance = std::make_shared<mtvk::Instance>(app_name);
	    if(instance->has_validation_layers()){
	    	debugging = std::make_shared<mtvk::VulkanDebug>(instance);
	    }
	}

	Renderer::~Renderer() {

	}
}