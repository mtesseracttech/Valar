//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_GRAPHICSPIPELINE_HPP
#define VALCANO_GRAPHICSPIPELINE_HPP

#include <gfx/vulkan/shaders/Shader.hpp>
#include <gfx/vulkan/display/Swapchain.hpp>
#include <gfx/vulkan/pipelines/RenderPass.hpp>

namespace mt::gfx::mtvk {
    class GraphicsPipeline {

        std::shared_ptr<Device> device;

        vk::PipelineLayout pipeline_layout;
        vk::Pipeline graphics_pipeline;

    public:
        GraphicsPipeline(
                const std::shared_ptr<Device>& device,
                const RenderPass& render_pass,
                const Swapchain& swapchain,
                const Shader& shader);

        ~GraphicsPipeline() = default;

        void destroy();
    };

}

#endif //VALCANO_GRAPHICSPIPELINE_HPP
