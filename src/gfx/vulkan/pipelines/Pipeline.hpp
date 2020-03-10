//
// Created by mtesseract on 06-02-20.
//

#include <gfx/vulkan/shaders/Shader.hpp>

#ifndef VALCANO_PIPELINE_HPP
#define VALCANO_PIPELINE_HPP

namespace mt::gfx::mtvk {
class Pipeline {
protected:
    std::shared_ptr<Device> device;
    std::shared_ptr<Shader> shader;

    vk::PipelineLayout pipeline_layout;
    vk::Pipeline pipeline;

    vk::PipelineBindPoint bind_point;

public:
    Pipeline(const std::shared_ptr<Device>& device,
        const std::shared_ptr<Shader>& shader)
        : device(device)
        , shader(shader)
    {
    }

    virtual ~Pipeline() = default;

    virtual void destroy()
    {
        device->get_device().destroyPipeline(pipeline);
        device->get_device().destroyPipelineLayout(pipeline_layout);
    }

    vk::Pipeline get_pipeline() const { return pipeline; }

    vk::PipelineBindPoint get_bind_point() const { return bind_point; }
};

} // namespace mt::gfx::mtvk

#endif // VALCANO_PIPELINE_HPP
