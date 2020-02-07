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
        std::shared_ptr<Swapchain> swapchain;
        std::shared_ptr<RenderPass> render_pass;

        std::vector<vk::Semaphore> image_available_semaphores;
        std::vector<vk::Semaphore> render_finished_semaphores;

        std::vector<vk::Fence> in_flight_fences;
        std::vector<vk::Fence> images_in_flight;

        uint32_t max_frames_in_flight = 1;

        std::size_t current_frame = 0;
    public:
        explicit CommandBuffer(const std::shared_ptr<Device>& device,
                               const std::shared_ptr<Swapchain>& swapchain,
                               const std::shared_ptr<RenderPass>& render_pass,
                               uint32_t max_frames_in_flight);

        ~CommandBuffer() = default;

        void destroy();

        void create_command_buffers(const Pipeline &pipeline);

        void submit_command_buffers();

    protected:

        void allocate_command_buffers();

        void create_sync_objects();
    };
}


#endif //VALCANO_COMMANDBUFFER_HPP
