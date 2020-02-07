//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_GRAPHICSPIPELINE_HPP
#define VALCANO_GRAPHICSPIPELINE_HPP


#include <gfx/vulkan/display/Swapchain.hpp>
#include <gfx/vulkan/pipelines/RenderPass.hpp>
#include "Pipeline.hpp"

namespace mt::gfx::mtvk {
    class GraphicsPipeline : public Pipeline {
    public:
        GraphicsPipeline(const std::shared_ptr<Device>& device,
                         const std::shared_ptr<Shader>& shader);

        void create(const RenderPass& render_pass, const Swapchain& swapchain);
    };

}

#endif //VALCANO_GRAPHICSPIPELINE_HPP
