//
// Created by mtesseract on 13-01-20.
//

#include <aux/logging/Logger.hpp>
#include <gfx/vulkan/display/Swapchain.hpp>
#include "CommandBuffer.hpp"
#include <array>

namespace mt::gfx::mtvk {
    CommandBuffer::CommandBuffer(const std::shared_ptr<Device> &device,
            const std::shared_ptr<Swapchain>& swapchain,
            const std::shared_ptr<RenderPass>& render_pass) : device(device), swapchain(swapchain), render_pass(render_pass){
        auto indices = device->get_queue_indices();

        vk::CommandPoolCreateInfo pool_create_info;
        pool_create_info.queueFamilyIndex = indices.graphics_family.value();
        pool_create_info.flags = vk::CommandPoolCreateFlags(0);

        command_pool = device->get_device().createCommandPool(pool_create_info);
        Logger::log("Created Command Pool", Info);

        create_command_buffers();

        create_semaphores();
    }

    void CommandBuffer::create_command_buffers() {
        auto swapchain_framebuffers = swapchain->get_framebuffers();

        command_buffers.resize(swapchain_framebuffers.size());

        vk::CommandBufferAllocateInfo command_buffer_allocate_info;
        command_buffer_allocate_info.commandPool = command_pool;
        command_buffer_allocate_info.level = vk::CommandBufferLevel::ePrimary;
        command_buffer_allocate_info.commandBufferCount = command_buffers.size();

        device->get_device().allocateCommandBuffers(&command_buffer_allocate_info, command_buffers.data());
        Logger::log("Allocated " + std::to_string(command_buffers.size()) + " Command Buffers", Info);
    }


    void CommandBuffer::destroy() {
        auto device = this->device->get_device();
        device.destroySemaphore(render_finished_semaphore);
        device.destroySemaphore(image_available_semaphore);
        device.destroyCommandPool(command_pool);
        Logger::log("Destroyed Command Pool", Info);
    }

    vk::CommandPool CommandBuffer::get_command_pool() const {
        return command_pool;
    }

    std::vector<vk::CommandBuffer> CommandBuffer::get_command_buffers() const {
        return command_buffers;
    }

    void CommandBuffer::create_semaphores() {
        //Could be added to Should be part of Command Buffer (and split Command Buffer into Single and Multi)
        vk::SemaphoreCreateInfo semaphore_info;
        image_available_semaphore = device->get_device().createSemaphore(semaphore_info);
        render_finished_semaphore = device->get_device().createSemaphore(semaphore_info);
        Logger::log("Created Semaphores", Info);
    }
}