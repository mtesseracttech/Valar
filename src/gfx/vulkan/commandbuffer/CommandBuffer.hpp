//
// Created by mtesseract on 13-01-20.
//

#ifndef VALCANO_COMMANDBUFFER_HPP
#define VALCANO_COMMANDBUFFER_HPP


#include <gfx/vulkan/pipelines/RenderPass.hpp>
#include <gfx/vulkan/pipelines/GraphicsPipeline.hpp>
#include "gfx/vulkan/display/Device.hpp"

namespace mt::gfx::mtvk {

    class CommandBuffer {
    protected:

        vk::CommandPool command_pool;
        std::vector<vk::CommandBuffer> command_buffers;

        std::shared_ptr<Device> device;

        std::vector<vk::Semaphore> image_available_semaphores;
        std::vector<vk::Semaphore> render_finished_semaphores;

        std::vector<vk::Fence> in_flight_fences;
        std::vector<vk::Fence> images_in_flight;

        uint32_t max_frames_in_flight = 1;

        std::size_t current_frame = 0;
    public:
        explicit CommandBuffer(const std::shared_ptr<Device>& device,
                               uint32_t max_frames_in_flight);

        ~CommandBuffer() = default;

        void create_command_pool();

        void allocate_command_buffers(const Swapchain& swapchain);

        void create_sync_objects(const Swapchain& swapchain);

        void create_command_buffers(const Pipeline& pipeline, const Swapchain& swapchain, const RenderPass& render_pass);

        void destroy();

        bool submit_command_buffers(const Swapchain& swapchain);

        void free_buffers();

    protected:

    };
}


#endif //VALCANO_COMMANDBUFFER_HPP
