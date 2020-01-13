//
// Created by mtesseract on 13-01-20.
//

#ifndef VALCANO_COMMANDBUFFER_HPP
#define VALCANO_COMMANDBUFFER_HPP


#include <gfx/vulkan/pipelines/RenderPass.hpp>
#include "Device.hpp"

namespace mt::gfx::mtvk {

    class CommandBuffer {
        vk::CommandPool command_pool;
        std::vector<vk::CommandBuffer> command_buffers;

        std::shared_ptr<Device> device;

    public:
        explicit CommandBuffer(const std::shared_ptr<Device>& device, const Swapchain& swapchain);
        ~CommandBuffer() = default;

        void destroy();

        vk::CommandPool get_command_pool() const;

        std::vector<vk::CommandBuffer> get_command_buffers() const;

        std::size_t get_buffer_count() const;

        vk::CommandBuffer get_buffer_at(std::size_t i) const;

        void create_command_buffers(const Swapchain &swapchain);
    };
}


#endif //VALCANO_COMMANDBUFFER_HPP
