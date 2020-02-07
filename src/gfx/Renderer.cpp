//
// Created by MTesseracT on 31/12/2019.
//

#include <aux/logging/Logger.hpp>
#include "Renderer.hpp"

namespace mt::gfx {
	Renderer::Renderer(const std::shared_ptr<ContextWindow>& render_window, const std::string& app_name, uint32_t max_frames_in_flight) :
            window(render_window),
            max_frames_in_flight(max_frames_in_flight){
        Logger::log("Creating the renderer", LogType::Info);
        instance = std::make_shared<mtvk::Instance>(app_name);
	    if(instance->has_validation_layers()){
	    	debugging = std::make_shared<mtvk::VulkanDebug>(instance);
	    }

	    surface = std::make_shared<mtvk::Surface>(instance);
	    surface->create(*window);

	    device = std::make_shared<mtvk::Device>(surface);
	    device->create(*instance);

	    swapchain = std::make_shared<mtvk::Swapchain>(device);
        swapchain->create(*surface, *window);

        render_pass = std::make_shared<mtvk::RenderPass>(device);
        render_pass->create(*swapchain);

        swapchain->create_framebuffers(*render_pass);

        test_shader = std::make_shared<mtvk::Shader>("base", device, mtvk::ShaderSourceType::GLSL);
        test_shader_pipeline = std::make_shared<mtvk::GraphicsPipeline>(device, test_shader);
        test_shader_pipeline->create(*render_pass, *swapchain);

	    command_buffer = std::make_shared<mtvk::CommandBuffer>(device, swapchain, render_pass, max_frames_in_flight);
	    command_buffer->create_command_buffers(*test_shader_pipeline);
	}

    void Renderer::draw() {
	    command_buffer->submit_command_buffers();
    }

    void Renderer::terminate(){
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

    void Renderer::on_resize(uint32_t new_width, uint32_t new_height) {
        //device->wait_till_idle();
	    //cleanup_swapchain();
        Logger::log("Framebuffer Resized to: " + std::to_string(new_width) + "x" + std::to_string(new_height));
    }

    void Renderer::recreate_swapchain() {
	    device->wait_till_idle();
    }

    void Renderer::cleanup_swapchain() {
        swapchain->destroy_framebuffers();
        //Free command buffers
        test_shader_pipeline->destroy();
        render_pass->destroy();
        swapchain->destroy();
    }
}