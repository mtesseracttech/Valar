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
        pipeline = std::make_shared<mtvk::GraphicsPipeline>(device, *render_pass, *swapchain, test_shader);
	    swapchain->create_framebuffers(*render_pass);
	    command_buffer = std::make_shared<mtvk::CommandBuffer>(device, swapchain, render_pass);
	}

    void Renderer::terminate(){
	    command_buffer->destroy();
	    swapchain->destroy_framebuffers();
	    pipeline->destroy();
	    render_pass->destroy();
        swapchain->destroy();
        device->destroy();
        surface->destroy();
        if(debugging) debugging->destroy();
        instance->destroy();
        Logger::log("Done terminating the renderer", Info);
    }

    void Renderer::draw() {
        Logger::log("Render starting now", Info);

        auto command_buffers = command_buffer->get_command_buffers();
        auto framebuffers = swapchain->get_framebuffers();

        for (std::size_t i = 0; i < command_buffers.size(); ++i) {
            vk::CommandBufferBeginInfo begin_info;
            begin_info.flags = {};
            begin_info.pInheritanceInfo = nullptr;

            command_buffers[i].begin(begin_info);

            vk::RenderPassBeginInfo render_pass_info;
            render_pass_info.renderPass = render_pass->get_render_pass();
            render_pass_info.framebuffer = framebuffers[i];

            render_pass_info.renderArea.offset = vk::Offset2D(0, 0);
            render_pass_info.renderArea.extent = swapchain->get_extent();

            vk::ClearValue clear_color = vk::ClearColorValue(std::array<float, 4>{1, 2, 3, 4});
            render_pass_info.clearValueCount = 1;
            render_pass_info.pClearValues = &clear_color;

            command_buffers[i].beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
            command_buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->get_pipeline());
            command_buffers[i].draw(3, 1, 0, 0);
            command_buffers[i].endRenderPass();
            command_buffers[i].end();
        }

        while(!window->should_close()){
            window->process_events();
	    }
        Logger::log("Render loop exited now", Info);
    }
}