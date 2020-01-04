//
// Created by MTesseracT on 31/12/2019.
//

#include <aux/logging/Logger.hpp>
#include "Renderer.hpp"
namespace mt::gfx {

	Renderer::Renderer(const std::shared_ptr<ContextWindow>& render_window, const std::string& app_name) : window(render_window){
        aux::Logger::log("Creating the renderer", aux::LogType::Info);

        instance = std::make_shared<mtvk::Instance>(app_name);
	    if(instance->has_validation_layers()){
	    	debugging = std::make_shared<mtvk::VulkanDebug>(instance);
	    }

	    surface = std::make_shared<mtvk::Surface>(render_window, instance);
	    device = std::make_shared<mtvk::Device>(instance, surface);
	    swapchain = std::make_shared<mtvk::Swapchain>(device , surface, window);

	    auto test_shader = mtvk::ShaderModule("base", device);
	}

    Renderer::~Renderer() {

    }
}