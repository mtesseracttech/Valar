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
	    render_pass = std::make_shared<mtvk::RenderPass>(device, *swapchain);
        auto test_shader = mtvk::Shader("base", device, mtvk::ShaderSourceType::GLSL);
        pipeline = std::make_shared<mtvk::GraphicsPipeline>(device, *render_pass, *swapchain, test_shader);
	    swapchain->create_framebuffers(*render_pass);
	    command_buffer = std::make_shared<mtvk::CommandBuffer>(device, *swapchain);
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
        aux::Logger::log("Done terminating the renderer", aux::LogType::Info);
    }

    void Renderer::draw() {
        auto frame_buffers = swapchain->get_framebuffers();

        for (std::size_t i = 0; i < command_buffer->get_buffer_count(); i++) {
            auto current_buffer = command_buffer->get_buffer_at(i);

            vk::CommandBufferBeginInfo begin_info;
            begin_info.flags = vk::CommandBufferUsageFlags(0);
            begin_info.pInheritanceInfo = nullptr;

            current_buffer.begin(begin_info);

            vk::RenderPassBeginInfo render_pass_info;
            render_pass_info.renderPass = render_pass->get_render_pass();
            render_pass_info.framebuffer = frame_buffers[i];
            render_pass_info.renderArea.offset = vk::Offset2D(0,0);
            render_pass_info.renderArea.extent = swapchain->get_extent();

            std::array<float, 4> clear_color = {0,0,0,1};
            vk::ClearValue color(clear_color);

            render_pass_info.clearValueCount = 1;
            render_pass_info.pClearValues = &color;

            current_buffer.beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
            current_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->get_pipeline());

            current_buffer.draw(3,1,0,0);

            current_buffer.endRenderPass();
            current_buffer.end();
        }
    }
}