//
// Created by MTesseracT on 31/12/2019.
//

#include <aux/logging/Logger.hpp>
#include "Renderer.hpp"

namespace mt::gfx {

	Renderer::Renderer(const std::shared_ptr<ContextWindow>& render_window, const std::string& app_name) : window(render_window){
        Logger::log("Creating the renderer", LogType::Info);

        instance = std::make_shared<mtvk::Instance>(app_name);
	    if(instance->has_validation_layers()){
	    	debugging = std::make_shared<mtvk::VulkanDebug>(instance);
	    }
	    surface = std::make_shared<mtvk::Surface>(instance, *window);
	    device = std::make_shared<mtvk::Device>(instance, surface);
	    swapchain = std::make_shared<mtvk::Swapchain>(device , *surface, *window);
	    render_pass = std::make_shared<mtvk::RenderPass>(device, *swapchain);
        auto test_shader = mtvk::Shader("base", device, mtvk::ShaderSourceType::GLSL);
        test_shader_pipeline = std::make_shared<mtvk::GraphicsPipeline>(device, test_shader, *render_pass, *swapchain);
	    swapchain->create_framebuffers(*render_pass);
	    command_buffer = std::make_shared<mtvk::CommandBuffer>(device, swapchain, render_pass);

	    command_buffer->create_command_buffers(*test_shader_pipeline);
	}

    void Renderer::draw() {
	    command_buffer->submit_command_buffers();


    }

    void Renderer::terminate(){
        device->wait_till_idle();
        //Manually doing this to make sure things get destroyed in the correct order,
        //I don't think there is a proper way to enforce this with C++'s smart pointers
        command_buffer->destroy();
        swapchain->destroy_framebuffers();
        test_shader_pipeline->destroy();
        render_pass->destroy();
        swapchain->destroy();
        device->destroy();
        surface->destroy();
        if(debugging) debugging->destroy();
        instance->destroy();
        Logger::log("Done terminating the renderer", Info);
    }
}