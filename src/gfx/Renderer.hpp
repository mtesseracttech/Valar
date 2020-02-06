//
// Created by MTesseracT on 31/12/2019.
//

#ifndef VALCANO_RENDERER_HPP
#define VALCANO_RENDERER_HPP

#include <memory>

#include <gfx/vulkan/display/Instance.hpp>
#include <gfx/window/ContextWindow.hpp>
#include <gfx/vulkan/debug/VulkanDebug.hpp>
#include <gfx/vulkan/display/Device.hpp>
#include <gfx/vulkan/display/Surface.hpp>
#include <gfx/vulkan/display/Swapchain.hpp>
#include <gfx/vulkan/shaders/Shader.hpp>
#include <gfx/vulkan/pipelines/GraphicsPipeline.hpp>
#include <gfx/vulkan/pipelines/RenderPass.hpp>
#include <gfx/vulkan/display/CommandBuffer.hpp>


namespace mt::gfx{
	class Renderer {
		std::shared_ptr<ContextWindow> window;
		std::shared_ptr<mtvk::VulkanDebug> debugging;
		std::shared_ptr<mtvk::Instance> instance;
		std::shared_ptr<mtvk::Device> device;
		std::shared_ptr<mtvk::Surface> surface;
		std::shared_ptr<mtvk::Swapchain> swapchain;
        std::shared_ptr<mtvk::RenderPass> render_pass;
        std::shared_ptr<mtvk::CommandBuffer> command_buffer;
        //Pipelines should be managed by a resource cache (with a pipeline cache)
        std::shared_ptr<mtvk::GraphicsPipeline> test_shader_pipeline;

    public:
		Renderer(const std::shared_ptr<ContextWindow>& render_window, const std::string& app_name);
		~Renderer() = default;

		void draw();

		void terminate();
	};
}



#endif //VALCANO_RENDERER_HPP
