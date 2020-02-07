//
// Created by MTesseracT on 31/12/2019.
//

#ifndef VALCANO_VULKANRENDERER_HPP
#define VALCANO_VULKANRENDERER_HPP

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
#include <gfx/vulkan/commandbuffer/CommandBuffer.hpp>
#include <gfx/Renderer.hpp>


namespace mt::gfx::mtvk {
	class VulkanRenderer : public Renderer {
		std::shared_ptr<VulkanDebug> debugging;
		std::shared_ptr<Instance> instance;
		std::shared_ptr<Device> device;
		std::shared_ptr<Surface> surface;
		std::shared_ptr<Swapchain> swapchain;
        std::shared_ptr<RenderPass> render_pass;
        std::shared_ptr<CommandBuffer> command_buffer;

        //Pipelines should be managed by a resource cache (with a pipeline cache)
        std::shared_ptr<GraphicsPipeline> test_shader_pipeline;
        std::shared_ptr<Shader> test_shader;

        uint32_t max_frames_in_flight = 2;
    public:
        explicit VulkanRenderer(const std::shared_ptr<ContextWindow>& render_window);
		~VulkanRenderer() = default;

		void draw();

		void terminate();

		void on_resize(uint32_t new_width, uint32_t new_height);

		void recreate_swapchain();

		void cleanup_swapchain();
	};
}



#endif //VALCANO_VULKANRENDERER_HPP
