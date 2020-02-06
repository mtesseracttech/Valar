//
// Created by mtesseract on 13-01-20.
//

#ifndef VALCANO_COMMANDBUFFER_HPP
#define VALCANO_COMMANDBUFFER_HPP


#include <gfx/vulkan/pipelines/RenderPass.hpp>
#include <gfx/vulkan/pipelines/GraphicsPipeline.hpp>
#include "Device.hpp"

namespace mt::gfx::mtvk {

    class CommandBuffer {
        vk::CommandPool command_pool;
        std::vector<vk::CommandBuffer> command_buffers;

        std::shared_ptr<Device> device;
        std::shared_ptr<Swapchain> swapchain;
        std::shared_ptr<RenderPass> render_pass;

        vk::Semaphore image_available_semaphore;
        vk::Semaphore render_finished_semaphore;

    public:
        explicit CommandBuffer(const std::shared_ptr<Device>& device, const std::shared_ptr<Swapchain>& swapchain, const std::shared_ptr<RenderPass>& render_pass);
        ~CommandBuffer() = default;

        void destroy();

        vk::CommandPool get_command_pool() const;

        std::vector<vk::CommandBuffer> get_command_buffers() const;

        void create_command_buffers();

        void create_semaphores();
    };
}


#endif //VALCANO_COMMANDBUFFER_HPP
