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

        vk::PipelineLayout pipeline_layout;
        vk::Pipeline pipeline;
    public:
        Pipeline(const std::shared_ptr<Device>& device, const Shader& shader) : device(device) {}

        virtual ~Pipeline(){};

        virtual void destroy(){
            device->get_device().destroyPipeline(pipeline);
            device->get_device().destroyPipelineLayout(pipeline_layout);
        }

        vk::Pipeline get_pipeline() const{
            return pipeline;
        }
    };

}

#endif //VALCANO_PIPELINE_HPP
