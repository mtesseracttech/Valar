//
// Created by mtesseract on 13-01-20.
//

#ifndef VALCANO_RENDERPASS_HPP
#define VALCANO_RENDERPASS_HPP

#include <gfx/vulkan/display/Device.hpp>
#include <gfx/vulkan/display/Swapchain.hpp>


namespace mt::gfx::mtvk {
    class RenderPass {
        std::shared_ptr<Device> device;

        vk::RenderPass render_pass;

    public:

        RenderPass(const std::shared_ptr<Device>& device, const std::weak_ptr<Swapchain>& swapchain);
        ~RenderPass() = default;

        void destroy();

        vk::RenderPass get_render_pass() const;
    };
}

#endif //VALCANO_RENDERPASS_HPP
