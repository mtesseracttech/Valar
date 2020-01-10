//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_GRAPHICSPIPELINE_HPP
#define VALCANO_GRAPHICSPIPELINE_HPP

#include <gfx/vulkan/shaders/Shader.hpp>
#include <gfx/vulkan/display/Swapchain.hpp>

namespace mt::gfx::mtvk {
    class GraphicsPipeline {
        Shader shader;
        std::shared_ptr<Device> device;
        std::weak_ptr<Swapchain> swapchain;

        vk::PipelineLayout pipeline_layout;
        vk::RenderPass render_pass;
        vk::Pipeline graphics_pipeline;

        void create_pipeline_layout();

        void create_render_pass();

        void create_pipeline(const Shader &shader);

    public:
        GraphicsPipeline(const Shader& shader, const std::shared_ptr<Device>& device, const std::weak_ptr<Swapchain>& swapchain);
        ~GraphicsPipeline();


    };

}

#endif //VALCANO_GRAPHICSPIPELINE_HPP
