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

	    surface = std::make_shared<mtvk::Surface>(instance, *window);

	    device = std::make_shared<mtvk::Device>(instance, surface);

	    swapchain = std::make_shared<mtvk::Swapchain>(device , *surface, *window);

	    render_pass = std::make_shared<mtvk::RenderPass>(device, swapchain);

        auto test_shader = mtvk::Shader("base", device, mtvk::ShaderSourceType::GLSL);

        pipeline = std::make_shared<mtvk::GraphicsPipeline>(device, *render_pass, *swapchain, test_shader);
	}

    Renderer::~Renderer() {

    }

    void Renderer::terminate(){
	    pipeline->destroy();
	    render_pass->destroy();
        swapchain->destroy();
        device->destroy();
        surface->destroy();
        if(debugging) debugging->destroy();
        instance->destroy();
    }
}