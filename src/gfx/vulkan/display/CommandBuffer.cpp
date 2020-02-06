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

        allocate_command_buffers();

        create_semaphores();
    }

    void CommandBuffer::allocate_command_buffers() {
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

    void CommandBuffer::create_command_buffers(const GraphicsPipeline& pipeline) {
        Logger::log("THIS IS A TEST COMMAND BUFFER", Error);
        auto frame_buffers = swapchain->get_framebuffers();

        for (std::size_t i = 0; i < command_buffers.size(); ++i) {
            vk::CommandBufferBeginInfo begin_info;
            begin_info.flags = {};
            begin_info.pInheritanceInfo = nullptr;

            command_buffers[i].begin(begin_info);

            vk::RenderPassBeginInfo render_pass_info;
            render_pass_info.renderPass = render_pass->get_render_pass();
            render_pass_info.framebuffer = frame_buffers[i];

            render_pass_info.renderArea.offset = vk::Offset2D(0, 0);
            render_pass_info.renderArea.extent = swapchain->get_extent();

            vk::ClearValue clear_color = vk::ClearColorValue(std::array<float, 4>{1, 2, 3, 4});
            render_pass_info.clearValueCount = 1;
            render_pass_info.pClearValues = &clear_color;

            //This will need a scenegraph as an input
            command_buffers[i].beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
            command_buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get_pipeline());
            command_buffers[i].draw(3, 1, 0, 0);
            command_buffers[i].endRenderPass();
            command_buffers[i].end();
        }
    }

    void CommandBuffer::submit_command_buffers() {
        uint32_t next_image_idx = device->get_device().acquireNextImageKHR(swapchain->get_swapchain(), UINT64_MAX, image_available_semaphore, nullptr).value;

        vk::SubmitInfo submit_info;

        vk::Semaphore wait_semaphores[] = {image_available_semaphore};
        vk::PipelineStageFlags wait_stages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffers[next_image_idx];

        vk::Semaphore signal_semaphores[] = {render_finished_semaphore};
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        device->get_graphics_queue().submit(submit_info, nullptr);


        vk::PresentInfoKHR present_info;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;

        vk::SwapchainKHR swapchains[] = {swapchain->get_swapchain()};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains;
        present_info.pImageIndices = &next_image_idx;
        present_info.pResults = nullptr;

        device->get_present_queue().presentKHR(&present_info);
        device->get_present_queue().waitIdle();
    }
}