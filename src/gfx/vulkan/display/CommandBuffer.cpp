//
// Created by mtesseract on 13-01-20.
//

#include <aux/logging/Logger.hpp>
#include <gfx/vulkan/display/Swapchain.hpp>
#include "CommandBuffer.hpp"
#include <array>

namespace mt::gfx::mtvk {
    CommandBuffer::CommandBuffer(const std::shared_ptr<Device> &device, const Swapchain& swapchain) : device(device){
        auto indices = device->get_queue_indices();

        vk::CommandPoolCreateInfo pool_create_info;
        pool_create_info.queueFamilyIndex = indices.graphics_family.value();
        pool_create_info.flags = vk::CommandPoolCreateFlags(0);

        command_pool = device->get_device().createCommandPool(pool_create_info);
        aux::Logger::log("Created Command Pool", aux::LogType::Info);

        create_command_buffers(swapchain);
    }

    void CommandBuffer::create_command_buffers(const Swapchain &swapchain) {
        command_buffers.resize(swapchain.get_framebuffers().size());

        vk::CommandBufferAllocateInfo command_buffer_allocate_info;
        command_buffer_allocate_info.commandPool = command_pool;
        command_buffer_allocate_info.level = vk::CommandBufferLevel::ePrimary;
        command_buffer_allocate_info.commandBufferCount = command_buffers.size();

        device->get_device().allocateCommandBuffers(&command_buffer_allocate_info, command_buffers.data());
        aux::Logger::log("Allocated " + std::to_string(command_buffers.size()) + " Command Buffers", aux::LogType::Info);
    }

    void CommandBuffer::destroy() {
        device->get_device().destroyCommandPool(command_pool);
        aux::Logger::log("Destroyed Command Pool", aux::LogType::Info);

    }

    vk::CommandPool CommandBuffer::get_command_pool() const {
        return command_pool;
    }

    std::vector<vk::CommandBuffer> CommandBuffer::get_command_buffers() const {
        return command_buffers;
    }

    vk::CommandBuffer CommandBuffer::get_buffer_at(std::size_t i) const {
        return command_buffers[i];
    }

    std::size_t CommandBuffer::get_buffer_count() const {
        return command_buffers.size();
    }
}