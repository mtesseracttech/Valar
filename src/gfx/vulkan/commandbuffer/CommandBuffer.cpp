//
// Created by mtesseract on 13-01-20.
//

#include <aux/logging/Logger.hpp>
#include <gfx/vulkan/display/Swapchain.hpp>
#include "CommandBuffer.hpp"
#include <array>

namespace mt::gfx::mtvk {
    CommandBuffer::CommandBuffer(const std::shared_ptr<Device> &device, uint32_t max_frames_in_flight) :
                device(device),
                max_frames_in_flight(max_frames_in_flight) {
    }

    void CommandBuffer::create_command_pool() {
        auto indices = device->get_queue_indices();

        vk::CommandPoolCreateInfo pool_create_info;
        pool_create_info.queueFamilyIndex = indices.graphics_family.value();
        pool_create_info.flags = vk::CommandPoolCreateFlags(0);

        command_pool = device->get_device().createCommandPool(pool_create_info);
        Logger::log("Created Command Pool", Info);
    }

    void CommandBuffer::allocate_command_buffers(const Swapchain& swapchain) {
        command_buffers.resize(swapchain.get_framebuffers().size());

        vk::CommandBufferAllocateInfo command_buffer_allocate_info;
        command_buffer_allocate_info.commandPool = command_pool;
        command_buffer_allocate_info.level = vk::CommandBufferLevel::ePrimary;
        command_buffer_allocate_info.commandBufferCount = command_buffers.size();

        device->get_device().allocateCommandBuffers(&command_buffer_allocate_info, command_buffers.data());
        Logger::log("Allocated " + std::to_string(command_buffers.size()) + " Command Buffers", Info);
    }

    void CommandBuffer::create_sync_objects(const Swapchain& swapchain) {
        image_available_semaphores.resize(max_frames_in_flight);
        render_finished_semaphores.resize(max_frames_in_flight);
        in_flight_fences.resize(max_frames_in_flight);
        images_in_flight.resize(swapchain.get_images().size(), nullptr);

        vk::SemaphoreCreateInfo semaphore_info;
        vk::FenceCreateInfo fence_info;
        fence_info.flags = vk::FenceCreateFlagBits::eSignaled;

        for(std::size_t i = 0; i < max_frames_in_flight; ++i){
            image_available_semaphores[i] = device->get_device().createSemaphore(semaphore_info);
            render_finished_semaphores[i] = device->get_device().createSemaphore(semaphore_info);
            in_flight_fences[i] = device->get_device().createFence(fence_info);
        }
        Logger::log("Created Semaphores", Info);
    }



    void CommandBuffer::destroy() {
        auto device = this->device->get_device();
        for(std::size_t i = 0; i < max_frames_in_flight; ++i){
            device.destroy(image_available_semaphores[i]);
            device.destroy(render_finished_semaphores[i]);
            device.destroy(in_flight_fences[i]);
        }
        device.destroyCommandPool(command_pool);
        Logger::log("Destroyed Command Pool", Info);
    }

    void CommandBuffer::create_command_buffers(const Pipeline& pipeline, const Swapchain& swapchain, const RenderPass& render_pass) {
        auto frame_buffers = swapchain.get_framebuffers();

        for (std::size_t i = 0; i < command_buffers.size(); ++i) {
            vk::CommandBufferBeginInfo begin_info;
            begin_info.flags = vk::CommandBufferUsageFlags();
            begin_info.pInheritanceInfo = nullptr;

            command_buffers[i].begin(begin_info);

            vk::RenderPassBeginInfo render_pass_info;
            render_pass_info.renderPass = render_pass.get_render_pass();
            render_pass_info.framebuffer = frame_buffers[i];

            render_pass_info.renderArea.offset = vk::Offset2D(0, 0);
            render_pass_info.renderArea.extent = swapchain.get_extent();

            vk::ClearValue clear_color = vk::ClearColorValue(std::array<float, 4>{0.0, 0.0, 0.0, 1.0});
            render_pass_info.clearValueCount = 1;
            render_pass_info.pClearValues = &clear_color;

            //This will need a scenegraph as an input
            command_buffers[i].beginRenderPass(render_pass_info, vk::SubpassContents::eInline);
            command_buffers[i].bindPipeline(pipeline.get_bind_point(), pipeline.get_pipeline());
            command_buffers[i].draw(3, 1, 0, 0);
            command_buffers[i].endRenderPass();
            command_buffers[i].end();
        }
    }

    bool CommandBuffer::submit_command_buffers(const Swapchain& swapchain) {
        device->get_device().waitForFences(1, &in_flight_fences[current_frame], true, UINT64_MAX);

        auto next_image = device->get_device().acquireNextImageKHR(swapchain.get_swapchain(), UINT64_MAX, image_available_semaphores[current_frame], nullptr);

        if(next_image.result == vk::Result::eErrorOutOfDateKHR){
            return false;
        }

        if(images_in_flight[next_image.value]){
            device->get_device().waitForFences(1, &images_in_flight[next_image.value], true, UINT64_MAX);
        }

        images_in_flight[next_image.value] = in_flight_fences[current_frame];

        vk::PipelineStageFlags wait_stages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        vk::Semaphore wait_semaphores[] = {image_available_semaphores[current_frame]};
        vk::Semaphore signal_semaphores[] = {render_finished_semaphores[current_frame]};

        vk::SubmitInfo submit_info;
        submit_info.waitSemaphoreCount      = 1;
        submit_info.pWaitSemaphores         = wait_semaphores;
        submit_info.pWaitDstStageMask       = wait_stages;
        submit_info.commandBufferCount      = 1;
        submit_info.pCommandBuffers         = &command_buffers[next_image.value];
        submit_info.signalSemaphoreCount    = 1;
        submit_info.pSignalSemaphores       = signal_semaphores;

        device->get_device().resetFences(1, &in_flight_fences[current_frame]);

        device->get_graphics_queue().submit(submit_info, in_flight_fences[current_frame]);

        vk::PresentInfoKHR present_info;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;

        vk::SwapchainKHR swapchains[] = {swapchain.get_swapchain()};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains;
        present_info.pImageIndices = &next_image.value;
        present_info.pResults = nullptr;

        device->get_present_queue().presentKHR(&present_info);
        device->get_present_queue().waitIdle();

        current_frame = (current_frame + 1) % max_frames_in_flight;
        return true;
    }

    void CommandBuffer::free_buffers() {
        device->get_device().freeCommandBuffers(command_pool,command_buffers);
    }
}